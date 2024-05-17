# Display all sub module library
This tool can help you resolve library function redefine in one driver issue
Note: this tool only for driver makefile, not include library makefile
Display as follows:
```
['DebugLib']
-----------------------
DxeDpcLib
-----------------------
Library Name is : DpcLib
  DebugLib

  UefiBootServicesTableLib

['DebugLib', 'UefiBootServicesTableLib']
-----------------------
DxeNetLib
-----------------------
Library Name is : NetLib
  BaseLib

  DebugLib

  BaseMemoryLib

  UefiBootServicesTableLib

  UefiRuntimeServicesTableLib

  UefiLib

  MemoryAllocationLib

  DevicePathLib

  PrintLib
...
...
...
```