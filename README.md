
---
## STM32F103C8T6 Breakdown:
- **STM32** = Arm-based 32-bit microcontroller (Device family)
- **F** = General-purpose (Product type)
- **103** = Performance line (Device subfamily)
- **C** = 48 pins (Pin count)
- **8** = 64 Kbytes of Flash memory
- **T** = LQFP (Package)
- **6** = Industrial temperature range, –40 to 85 °C (Temperature range)


---

## Using ST-Link with WSL

To use ST-Link with WSL, follow these steps:

1. **Open PowerShell with Administrative Access**  
   Start PowerShell with administrator privileges. You can do this by right-clicking the Start menu and selecting **Windows Terminal (Admin)** or **PowerShell (Admin)**.

2. **List Available USB Devices**  
   To view the list of USB devices connected to your system, run the following command:

   ```powershell
   $usbipd list
   ```

3. **Bind the USB Device**  
   Identify the `BusID` of your ST-Link from the list and bind it using the command below. Replace `2-2` with the actual `BusID` of your device.

   ```powershell
   $usbipd bind --busid 2-2
   ```

4. **Verify the Device is Shared**  
   To ensure that the device is shared, re-run the list command:

   ```powershell
   $usbipd list
   ```

   Look for the status of the device to confirm it is shared.

5. **Attach the Device to WSL**  
   Finally, attach the USB device to your WSL environment by running:

   ```powershell
   $usbipd attach --wsl --busid 2-2
   ```

   After this, your ST-Link device should be accessible within your WSL environment.

For more details, refer to the [official documentation](https://learn.microsoft.com/en-us/windows/wsl/connect-usb).


---

## Building the Executable

To compile the code for your STM32, use the following command:

```bash
$ make all
```

This command compiles

---

## Flashing

To flash the compiled code onto your STM32, use OpenOCD with the following command:

```bash
$ make flash
```

This command connects and flash to the STM32 via ST-Link,

---

## Cleaning

To remove all build files:

```bash
$ make clean
```
---