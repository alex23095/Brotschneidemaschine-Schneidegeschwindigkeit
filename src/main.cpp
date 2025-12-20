#include "userInterface.hpp"
#include "mainControlUnit.hpp"
#include <iostream>
#include <chrono>
#include <thread>
#include "csvLogger.hpp"
#include <filesystem>
#include <iostream>


static void waitUntilReached(MainControlUnit& mcu, UserInterface& ui) {
	constexpr int MAX_RPM = 3000;      // muss zu deinem SensorSim::maxRpm_ passen
	constexpr int TOL_RPM = 5;         // Toleranz gegen „Endlos-wackeln“
	constexpr int TICK_MS = 100;

	while (true) {
		mcu.tick();
		const Status& st = mcu.getStatus();

		// Abbruchbedingungen
		if (st.safetyState != SafetyState::Safe) break;
		//if (st.runState != RunState::Run) break;

		const int targetRpm = (MAX_RPM * st.speedSetpointPct) / 100;
		const int diff = (st.speedActualRpm > targetRpm) ? (st.speedActualRpm - targetRpm)
			: (targetRpm - st.speedActualRpm);
		// Anzeige aktualisieren (ja, das flackert etwas – aber nur während der kurzen Rampe)
		ui.clearScreen();
		ui.showHeader(st);
		ui.showMotorMenu();

		if (diff <= TOL_RPM) break;

		std::this_thread::sleep_for(std::chrono::milliseconds(TICK_MS));
	}
}
static void waitUntilRpmZero(MainControlUnit& mcu, UserInterface& ui) {
	constexpr int TOL_RPM = 0;     // wirklich 0
	constexpr int TICK_MS = 100;
	constexpr int MAX_ITERS = 200; // Sicherheitsnetz: max 20s

	for (int i = 0; i < MAX_ITERS; ++i) {
		mcu.tick();
		const Status& st = mcu.getStatus();

		// Safety -> sofort raus
		if (st.safetyState != SafetyState::Safe) return;

		// Nur Header zeigen (weniger Flackern)
		ui.clearScreen();
		ui.showHeader(st);

		if (st.speedActualRpm <= TOL_RPM) return;

		std::this_thread::sleep_for(std::chrono::milliseconds(TICK_MS));
	}
}



int main() {
	UserInterface ui;
	SafetyInput safety;
	MotorActuator motor;
	CurrentSensor currentSensor;
	MonitoringService monitoring;
	MainControlUnit mcu(motor, safety, currentSensor, monitoring);
	CsvLogger logger("machine_log.csv", 100 * 1024); // z.B. 100 KB


	bool running = true;
	int speedStep = 0;

	while (running) {
		ui.clearScreen();
		ui.showHeader(mcu.getStatus());
		ui.showMainMenu();
		int choice = ui.readInt();

		switch (choice) {
		case 1: // Safety-Menü
		{ 
			bool back = false;
			while (!back) {
				ui.clearScreen();
				ui.showHeader(mcu.getStatus());
				ui.showSafetyMenu();
				int c = ui.readInt();
				if (c == 1) {
					safety.setSafetyInput(SafetyState::Safe);
					mcu.tick();
					break;
				}
				else if (c == 2) {
					safety.setSafetyInput(SafetyState::EstopActive);
					mcu.tick();
					break;
				}
				else if (c == 3) {
					safety.setSafetyInput(SafetyState::GuardOpen);
					mcu.tick();
					break;
				}
				else if (c == 0) {
					back = true;
				}
				logger.logStatus(mcu.getStatus());
			}
			break;
		}
		case 2: // Motor-Menü
		{ 
			if (mcu.getStatus().runState == RunState::Run) {
				bool back = false;
				while (!back) {
					ui.clearScreen();
					ui.showHeader(mcu.getStatus());
					ui.showMotorMenu();
					char c = ui.readChar();
					if (c == '+') {
						if (speedStep < 10) speedStep++;
						mcu.setSpeedStep(speedStep);
						waitUntilReached(mcu, ui);
					}
					else if (c == '-') {
						if (speedStep > 0) speedStep--;
						mcu.setSpeedStep(speedStep);
						waitUntilReached(mcu, ui);
					}
					else if (c == '0') {
						back = true;
					}
					mcu.tick(); //Änderungen übernehmen
					logger.logStatus(mcu.getStatus());
				}
				break;
			}
			break;
		}
		case 3: // Motor starten
			ui.clearScreen();
			mcu.start();
			mcu.tick(); // Änderungen übernehmen   
			logger.logStatus(mcu.getStatus());
			ui.showHeader(mcu.getStatus());
			break;

		case 4: // Motor stoppen 
			ui.clearScreen();
			mcu.stop();
			waitUntilRpmZero(mcu, ui);
			logger.logStatus(mcu.getStatus());
			break;

		case 5: // Wartung
		{
			bool back = false;
			while (!back) {
				ui.clearScreen();
				ui.showHeader(mcu.getStatus());
				ui.showMaintenanceMenu(mcu.getStatus());
				int c = ui.readInt();
				if (c == 1) {
					ui.clearScreen();
					ui.showHeader(mcu.getStatus());
					ui.showMaintenanceConfig(mcu.getStatus());
					std::uint64_t limitMs = static_cast<std::uint64_t>(ui.readInt());
					mcu.setMaintenanceLimitMs(limitMs);
					mcu.tick();
					logger.logStatus(mcu.getStatus());
				}
				else if (c == 2) {
					ui.clearScreen();
					ui.showHeader(mcu.getStatus());

					const bool ok = mcu.runSelfTest();
					std::cout << (ok ? "Self-Test ok \n" : "Self-Test Fehlerhaft\n");
					ui.waitSeconds(2);
				}
				else if (c == 0) { back = true; }
			}
			break;
		}
		case 6:  // Daten-Handler
		{
			bool back = false;
			while (!back) {
				ui.clearScreen();
				ui.showHeader(mcu.getStatus());
				ui.showDataHandlerMenu();
				int c = ui.readInt();
				if (c == 0) back = true;
				if (c == 1) {
					mcu.tick(); // Zustand einmal aktualisieren
					logger.logStatus(mcu.getStatus());
					std::cout << "Status geloggt.\n";
					ui.waitSeconds(1);
				}
			}
			break;
		}

		case 7: { //Simulation
			bool back = false;
			while (!back) {
				ui.clearScreen();
				ui.showHeader(mcu.getStatus());

				// simulierten Stomwert für Overcurrent vorgeben
				std::cout << "Simulierter Motorstrom in mA (Grenzwert: 5000mA): ";
				int mA = ui.readInt();
				// beschreiben
				currentSensor.setSimCurrentmA(mA);

				mcu.tick(); // direkt auswerten + Headerwerte aktualisieren
				logger.logStatus(mcu.getStatus());

				ui.clearScreen();
				ui.showHeader(mcu.getStatus());
				ui.waitSeconds(2);
				break;
			}
			break;
		}
		case 0:
			running = false;
			break;

		default:
			ui.clearScreen();
			ui.showHeader(mcu.getStatus());
			std::cout << "Ungueltige Auswahl.\n";
			ui.waitSeconds(2);
			break;
		}
	}

	return 0;
}
