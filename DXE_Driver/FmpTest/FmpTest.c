/** @file
  Test flash test.bin use FMP

  Copyright (c) 2024, Insyde Corporation. All rights reserved.<BR>
**/

#include <Uefi.h>
#include <Library/UefiLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/DebugLib.h>
#include <Protocol/FirmwareManagement.h>
#include <Library/BaseMemoryLib.h>
#include <Guid/FmpCapsule.h>

EFI_FIRMWARE_MANAGEMENT_PROTOCOL  mTestFirmwareManagement;
EFI_FIRMWARE_IMAGE_DESCRIPTOR     MyDescriptor;
EFI_GUID MyFmpGuid = {0xC67C0AB3, 0x963A, 0x42FB, {0xB8,0x90,0x49, 0x3C, 0x3C, 0xB0, 0x84, 0x37}};

EFI_STATUS
EFIAPI
MyCheckImage (
  IN     EFI_FIRMWARE_MANAGEMENT_PROTOCOL       *This,
  IN     UINT8                                  ImageIndex,
  IN     CONST VOID                             *Capsule,
  IN     UINTN                                  CapsuleSize,
  OUT    UINT32                                 *ImageUpdatable
  )
{
  EFI_STATUS  Status = EFI_UNSUPPORTED;
  EFI_CAPSULE_HEADER *CapsuleHeader = NULL;

  DEBUG ((DEBUG_INFO, "MyCheckImage\n"));
  
  CapsuleHeader = (EFI_CAPSULE_HEADER*)Capsule;

  DEBUG ((DEBUG_INFO, "MyCapsuleGuid: %g", CapsuleHeader->CapsuleGuid));

  if (CompareGuid (&CapsuleHeader->CapsuleGuid, &MyFmpGuid) == TRUE) {
    DEBUG ((DEBUG_INFO, "Find the mytestfmp\n"));
    Status = EFI_SUCCESS;
  } else {
    DEBUG ((DEBUG_INFO, "Not find the mytestfmp\n"));
  }

  return Status;
}

EFI_STATUS
EFIAPI
MySetImage (
  IN     EFI_FIRMWARE_MANAGEMENT_PROTOCOL               *This,
  IN     UINT8                                          ImageIndex,
  IN     CONST VOID                                     *Capsule,
  IN     UINTN                                          CapsuleSize,
  IN     CONST VOID                                     *VendorCode,
  IN     EFI_FIRMWARE_MANAGEMENT_UPDATE_IMAGE_PROGRESS  Progress,
  OUT    CHAR16                                         **AbortReason
  )
{
  EFI_STATUS  Status = EFI_SUCCESS;
  EFI_FIRMWARE_MANAGEMENT_CAPSULE_HEADER        *FmpCapsuleHeader;
  DEBUG ((DEBUG_INFO, "MySetImage\n"));

  FmpCapsuleHeader = (EFI_FIRMWARE_MANAGEMENT_CAPSULE_HEADER *)((UINT8 *)Capsule + ((EFI_CAPSULE_HEADER *)Capsule)->HeaderSize);
  return Status;
}

EFI_STATUS
EFIAPI
MyGetImage (
  IN  EFI_FIRMWARE_MANAGEMENT_PROTOCOL  *This,
  IN  UINT8                             ImageIndex,
  OUT  VOID                             *Image,
  IN  OUT  UINTN                        *ImageSize
  )
{
  EFI_STATUS  Status = EFI_SUCCESS;
  DEBUG ((DEBUG_INFO, "MyGetImage\n"));
  return Status;
}

EFI_STATUS
EFIAPI
MyGetImageInfo (
  IN EFI_FIRMWARE_MANAGEMENT_PROTOCOL       *This,
  IN OUT    UINTN                           *ImageInfoSize,
  IN OUT    EFI_FIRMWARE_IMAGE_DESCRIPTOR   *ImageInfo,
  OUT       UINT32                          *DescriptorVersion,
  OUT       UINT8                           *DescriptorCount,
  OUT       UINTN                           *DescriptorSize,
  OUT       UINT32                          *PackageVersion,
  OUT       CHAR16                          **PackageVersionName
  )
{

  DEBUG ((DEBUG_INFO, "MyGetImageInfo\n"));

  // Insert Esrt need to check ImageInfoSize
  if (ImageInfoSize == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  if (*ImageInfoSize == 0) {
    *ImageInfoSize = sizeof (EFI_FIRMWARE_IMAGE_DESCRIPTOR);
    return EFI_BUFFER_TOO_SMALL;
  }
  CopyMem (&ImageInfo ,&MyDescriptor, sizeof (EFI_FIRMWARE_IMAGE_DESCRIPTOR));

  *DescriptorVersion = EFI_FIRMWARE_IMAGE_DESCRIPTOR_VERSION;
  // This design for Esrt 
  *DescriptorCount   = 1;
  *DescriptorSize    = sizeof (EFI_FIRMWARE_IMAGE_DESCRIPTOR);

  return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
MyGetPackageInfo (
  IN  EFI_FIRMWARE_MANAGEMENT_PROTOCOL *This,
  OUT UINT32                           *PackageVersion,
  OUT CHAR16                           **PackageVersionName,
  OUT UINT32                           *PackageVersionNameMaxLen,
  OUT UINT64                           *AttributesSupported,
  OUT UINT64                           *AttributesSetting
  )
{
  DEBUG ((DEBUG_INFO, "MyGetPackageInfo\n"));
  return EFI_UNSUPPORTED;
}

EFI_STATUS
EFIAPI
MySetPackageInfo (
  IN  EFI_FIRMWARE_MANAGEMENT_PROTOCOL   *This,
  IN  CONST VOID                         *Image,
  IN  UINTN                              ImageSize,
  IN  CONST VOID                         *VendorCode,
  IN  UINT32                             PackageVersion,
  IN  CONST CHAR16                       *PackageVersionName
  )
{
  DEBUG ((DEBUG_INFO, "MySetPackageInfo\n"));
  return EFI_UNSUPPORTED;
}

VOID
FillMyDescriptor (
  OUT EFI_FIRMWARE_IMAGE_DESCRIPTOR          *Descriptor
  )
{
  ZeroMem (Descriptor, sizeof (EFI_FIRMWARE_IMAGE_DESCRIPTOR));
  // If the descriptor has the IN USE bit set, create ESRT entry otherwise ignore.
  Descriptor->AttributesSupported         = (IMAGE_ATTRIBUTE_IMAGE_UPDATABLE | IMAGE_ATTRIBUTE_RESET_REQUIRED | IMAGE_ATTRIBUTE_IN_USE);
  Descriptor->AttributesSetting           = (IMAGE_ATTRIBUTE_IMAGE_UPDATABLE | IMAGE_ATTRIBUTE_RESET_REQUIRED | IMAGE_ATTRIBUTE_IN_USE);

  CopyGuid (&Descriptor->ImageTypeId, &MyFmpGuid);
}

/**
  My Fmp test entry

  @param[in] ImageHandle    The firmware allocated handle for the EFI image.
  @param[in] SystemTable    A pointer to the EFI System Table.

  @retval EFI_SUCCESS       The entry point is executed successfully.
  @retval other             Some error occurs when executing this entry point.

**/
EFI_STATUS
EFIAPI
FmpTestEntry (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  EFI_STATUS      Status = EFI_SUCCESS;
  EFI_HANDLE      Handle = NULL;
  DEBUG ((DEBUG_INFO, "FmpTestEntry\n"));

  FillMyDescriptor (&MyDescriptor);

  mTestFirmwareManagement.CheckImage =   MyCheckImage;
  mTestFirmwareManagement.SetImage   =   MySetImage;
  mTestFirmwareManagement.GetImage   =   MyGetImage;
  mTestFirmwareManagement.GetImageInfo = MyGetImageInfo;
  mTestFirmwareManagement.GetPackageInfo = MyGetPackageInfo;
  mTestFirmwareManagement.SetPackageInfo = MySetPackageInfo;


  Status = gBS->InstallProtocolInterface (
                  &Handle,
                  &gEfiFirmwareManagementProtocolGuid, 
                  EFI_NATIVE_INTERFACE,
                  &mTestFirmwareManagement);
  return Status;
}