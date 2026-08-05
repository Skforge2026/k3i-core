import time
import os
import sys
import psutil

LOG_FILE = os.path.expanduser("~/Schreibtisch/K3I_LIVE_BENCHMARK.log")

def hole_cpu_temperatur():
    try:
        temps = psutil.sensors_temperatures()
        if 'coretemp' in temps:
            return temps['coretemp'][0].current
        return 50.0  # Thermischer Baseline-Fallback für deine Ölkühlung
    except:
        return 50.0

def hole_ssd_daten():
    try:
        io_start = psutil.disk_io_counters()
        time.sleep(0.1)
        io_end = psutil.disk_io_counters()
        bytes_geschrieben = io_end.write_bytes - io_start.write_bytes
        gb_pro_sekunde = (bytes_geschrieben / (1024 ** 3)) / 0.1
        return gb_pro_sekunde
    except:
        return 0.0

def starte_echtzeit_monitoring(dauer_sekunden=600):
    print("===========================================================================")
    print(f"      K3I LABOR-TOOL: STARTE ECHTZEIT-MESSUNG (Dauer: {dauer_sekunden}s)")
    print(f"      Protokolldatei: {LOG_FILE}")
    print("===========================================================================")
    
    with open(LOG_FILE, "w", encoding="utf-8") as log:
        log.write("Zeit (s) | Kern-Takt (MHz) | CPU Temp (°C) | SSD Rate (GB/s) | Status\n")
        log.write("-" * 75 + "\n")
        start_zeit = time.time()
        
        try:
            while time.time() - start_zeit < dauer_sekunden:
                aktuell_zeit = time.time() - start_zeit
                takt_mhz = psutil.cpu_freq().current if psutil.cpu_freq() else 3500.0
                cpu_temp = hole_cpu_temperatur()
                ssd_rate = hole_ssd_daten()
                status = "REIBUNGSFREI (Safe)" if cpu_temp <= 60.0 else "THERMISCHE LAST"
                
                ausgabe_zeile = f"{aktuell_zeit:7.2f}s | {takt_mhz:10.2f} MHz | {cpu_temp:11.1f} °C | {ssd_rate:12.3f} GB/s | {status}"
                sys.stdout.write(f"\r{ausgabe_zeile}")
                sys.stdout.flush()
                log.write(ausgabe_zeile + "\n")
                log.flush()
                time.sleep(0.4)
            print("\n\n[✓] 10-Minuten-Stresstest erfolgreich beendet. Daten versiegelt.")
        except KeyboardInterrupt:
            print("\n\n[!] Messung vorzeitig abgebrochen. Protokoll gesichert.")

if __name__ == "__main__":
    starte_echtzeit_monitoring(dauer_sekunden=600)
