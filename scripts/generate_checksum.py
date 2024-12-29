from os.path import join
import hashlib

Import("env")  # Importiere die PlatformIO-Umgebung

def generate_checksum(source, target, env):
    firmware_path = join(env.subst("$BUILD_DIR"), "firmware.bin")
    print(f"Generating checksum for: {firmware_path}")
    try:
        with open(firmware_path, "rb") as f:
            file_data = f.read()
            checksum = hashlib.sha256(file_data).hexdigest()
            print(f"Checksum (SHA256): {checksum}")
    except FileNotFoundError:
        print(f"Firmware file not found: {firmware_path}")

# Füge eine Post-Build-Aktion hinzu, um die Checksumme nach der Erstellung der firmware.bin zu generieren
env.AddPostAction("$BUILD_DIR/firmware.bin", generate_checksum)
