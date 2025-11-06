
@startuml
title Kommunikationsdiagramm: Bedienung • Überwachung • Safety (vereinfacht und lesbar)

' ===== Darstellungseinstellungen =====
left to right direction
skinparam defaultFontSize 18
skinparam ArrowFontSize 16
skinparam ObjectFontSize 20
skinparam LegendFontSize 16
skinparam Padding 40
skinparam linetype ortho
skinparam shadowing false
skinparam roundcorner 20

' ===== Objekte =====
object User
object UI  as "UserInterfaceService\n(UI)"
object MCU as "MainControlUnit\n(Control Logic)"
object MC  as "MotorController\n(Hardware)"
object MS  as "MonitoringService\n(Control Logic)"
object SCU as "SafetyControlUnit\n(Safety)"
object DM  as "DataManagement\n(Persistence)"

' ===== Leerräume zur Streckung =====
User -[hidden]-> UI
UI -[hidden]-> MCU
MCU -[hidden]-> MC
MC -[hidden]-> MS
MS -[hidden]-> SCU
SCU -[hidden]-> DM

' ===== Nachrichten =====
' 1) Drehzahl setzen
User --> UI  : 1.1
UI   --> MCU : 1.2
MCU  --> MC  : 1.3
MC   --> MCU : 1.4
MCU  --> DM  : 1.5
MCU  --> UI  : 1.6

' 2) Überwachung / Wartung
MCU --> MS  : 2.1
MS  --> MCU : 2.2
MS  --> UI  : 2.3
MCU --> DM  : 2.4

' 3) Safety-Stop
SCU --> MCU : 3.1
MCU --> MC  : 3.2
MC  --> MCU : 3.3
MCU --> UI  : 3.4
MCU --> DM  : 3.5

' ===== Legende =====
legend right
<b>1.x – Drehzahl setzen</b>\n
1.1 Benutzer wählt Drehzahlschritt (UI-Eingabe)\n
1.2 UI sendet neuen Sollwert an MainControlUnit\n
1.3 MainControlUnit gibt Sollwert an MotorController weiter\n
1.4 MotorController bestätigt Regelung aktiv\n
1.5 Ereignis „SpeedChange“ wird im DataManagement geloggt\n
1.6 Statusanzeige im UI wird aktualisiert\n
\n
<b>2.x – Überwachung & Wartung</b>\n
2.1 MainControlUnit fordert Statusdaten vom MonitoringService an\n
2.2 MonitoringService sendet Restlebensdauer (RUL%) zurück\n
2.3 UI zeigt Wartungshinweis an, falls RUL < 10 %\n
2.4 Ereignis „MaintenanceAdvice“ wird geloggt\n
\n
<b>3.x – Safety-Stop</b>\n
3.1 SafetyControlUnit meldet Sicherheitsauslösung (TRIPPED)\n
3.2 MainControlUnit löst Not-Stopp am MotorController aus\n
3.3 MotorController bestätigt Stillstand\n
3.4 UI zeigt Stoppmeldung an\n
3.5 Ereignis „E-STOP“ wird im DataManagement gespeichert
end legend

@enduml
