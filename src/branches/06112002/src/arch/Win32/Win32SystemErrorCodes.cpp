#include <windows.h>

//	A comprehensive list of System Error Codes that win32 uses
//	to report errors with, blimey this took a long time to write...
char * GetWin32SystemErrorCode(DWORD ErrorCode)
{
	switch(ErrorCode){
		case EPT_S_CANT_CREATE:
			return "The endpoint mapper database entry could not be created.\0";
		case EPT_S_CANT_PERFORM_OP:
			return "The server endpoint cannot perform the operation.\0";
		case EPT_S_INVALID_ENTRY:
			return "The entry is invalid.\0";
		case EPT_S_NOT_REGISTERED:
			return "There are no more endpoints available from the endpoint mapper.\0";
		case ERROR_ACCESS_DENIED:
			return "Access is denied.\0";
    case ERROR_ACCOUNT_DISABLED:
			return "Logon failure: account currently disabled.\0";
		case ERROR_ACCOUNT_EXPIRED:
			return "The user's account has expired.\0";
		case ERROR_ACCOUNT_LOCKED_OUT:
			return "The referenced account is currently locked out and may not be logged on to.\0";
		case ERROR_ACCOUNT_RESTRICTION:
			return "Logon failure: user account restriction.\0";
		case ERROR_ACTIVE_CONNECTIONS:
			return "Active connections still exist.\0";
		case ERROR_ADAP_HDW_ERR:
			return "A network adapter hardware error occurred.\0";
		case ERROR_ADDRESS_ALREADY_ASSOCIATED:
			return "The network transport endpoint already has an address associated with it.\0";
		case ERROR_ADDRESS_NOT_ASSOCIATED:
			return "An address has not yet been associated with the network endpoint.\0";
		case ERROR_ALIAS_EXISTS:
			return "The specified local group already exists.\0";
		case ERROR_ALLOTTED_SPACE_EXCEEDED:
			return "No more memory is available for security information updates.\0";
		case ERROR_ALREADY_ASSIGNED:
			return "The local device name is already in use.\0";
		case ERROR_ALREADY_EXISTS:
			return "Cannot create a file when that file already exists.\0";
		case ERROR_ALREADY_INITIALIZED:
			return "An attempt was made to perform an initialization operation when initialization has already been completed.\0";
		case ERROR_ALREADY_REGISTERED:
			return "The service is already registered.\0";
		case ERROR_ALREADY_RUNNING_LKG:
			return "The system is currently running with the last-known-good configuration.\0";
		case ERROR_ALREADY_WAITING:
			return "The specified printer handle is already being waited on.\0";
		case ERROR_APP_WRONG_OS:
			return "The specified program is not a Windows or MS-DOS program.\0";
		case ERROR_ARENA_TRASHED:
			return "The storage control blocks were destroyed.\0";
		case ERROR_ARITHMETIC_OVERFLOW:
			return "Arithmetic result exceeded 32 bits.\0";
		case ERROR_ATOMIC_LOCKS_NOT_SUPPORTED:
			return "The file system does not support atomic changes to the lock type.\0";
		case ERROR_AUTODATASEG_EXCEEDS_64k:
			return "The operating system cannot run this application program.\0";
		case ERROR_BAD_ARGUMENTS:
			return "The argument string passed to DosExecPgm is not correct.\0";
		case ERROR_BAD_COMMAND:
			return "The device does not recognize the command.\0";
		case ERROR_BAD_CONFIGURATION:
			return "Configuration data corrupt.\0";
		case ERROR_BAD_DATABASE_VERSION:
			return "Database version unsupported.\0";
		case ERROR_BAD_DESCRIPTOR_FORMAT:
			return "A security descriptor is not in the right format (absolute or self-relative).\0";
		case ERROR_BAD_DEV_TYPE:
			return "The network resource type is not correct.\0";
		case ERROR_BAD_DEVICE:
			return "The specified device name is invalid.\0";
		case ERROR_BAD_DRIVER:
			return "The specified driver is invalid.\0";
		case ERROR_BAD_DRIVER_LEVEL:
			return "The system does not support the command requested.\0";
		case ERROR_BAD_ENVIRONMENT:
			return "The environment is incorrect.\0";
		case ERROR_BAD_EXE_FORMAT:
			return "is not a valid Win32 application.\0";
		case ERROR_BAD_FORMAT:
			return "An attempt was made to load a program with an incorrect format.\0";
		case ERROR_BAD_IMPERSONATION_LEVEL:
			return "Either a required impersonation level was not provided, or the provided impersonation level is invalid.\0";
		case ERROR_BAD_INHERITANCE_ACL:
			return "The inherited access control list (ACL) or access control entry (ACE) could not be built.\0";
		case ERROR_BAD_LENGTH:
			return "The program issued a command but the command length is incorrect.\0";
		case ERROR_BAD_LOGON_SESSION_STATE:
			return "The logon session is not in a state that is consistent with the requested operation.\0";
		case ERROR_BAD_NET_NAME:
			return "The network name cannot be found.\0";
		case ERROR_BAD_NET_RESP:
			return "The specified server cannot perform the requested operation.\0";
		case ERROR_BAD_NETPATH:
			return "The network path was not found.\0";
		case ERROR_BAD_PATHNAME:
			return "The specified path is invalid.\0";
		case ERROR_BAD_PIPE:
			return "The pipe state is invalid.\0";
		case ERROR_BAD_PROFILE:
			return "The network connection profile is corrupted.\0";
		case ERROR_BAD_PROVIDER:
			return "The specified network provider name is invalid.\0";
		case ERROR_BAD_QUERY_SYNTAX:
			return "SQL query syntax invalid or unsupported.\0";
		case ERROR_BAD_REM_ADAP:
			return "The remote adapter is not compatible.\0";
		case ERROR_BAD_THREADID_ADDR:
			return "The address for the thread ID is not correct.\0";
		case ERROR_BAD_TOKEN_TYPE:
			return "The type of the token is inappropriate for its attempted use.\0";
		case ERROR_BAD_UNIT:
			return "The system cannot find the device specified.\0";
		case ERROR_BAD_USERNAME:
			return "The specified username is invalid.\0";
		case ERROR_BAD_VALIDATION_CLASS:
			return "The validation information class requested was invalid.\0";
		case ERROR_BADDB:
			return "The configuration registry database is corrupt.\0";
		case ERROR_BADKEY:
			return "The configuration registry key is invalid.\0";
		case ERROR_BEGINNING_OF_MEDIA:
			return "The beginning of the tape or a partition was encountered.\0";
		case ERROR_BOOT_ALREADY_ACCEPTED:
			return "The current boot has already been accepted for use as the last-known-good control set.\0";
		case ERROR_BROKEN_PIPE:
			return "The pipe has been ended.\0";
		case ERROR_BUFFER_OVERFLOW:
			return "The file name is too long.\0";
		case ERROR_BUS_RESET:
			return "The I/O bus was reset.\0";
		case ERROR_BUSY:
			return "The requested resource is in use.\0";
		case ERROR_BUSY_DRIVE:
			return "The system cannot perform a JOIN or SUBST at this time.\0";
		case ERROR_CALL_NOT_IMPLEMENTED:
			return "This function is not supported on this system.\0";
		case ERROR_CAN_NOT_COMPLETE:
			return "Cannot complete this function.\0";
		case ERROR_CAN_NOT_DEL_LOCAL_WINS:
			return "The local WINS can not be deleted.\0";
		case ERROR_CANCEL_VIOLATION:
			return "A lock request was not outstanding for the supplied cancel region.\0";
		case ERROR_CANCELLED:
			return "The operation was canceled by the user.\0";
		case ERROR_CANNOT_COPY:
			return "The copy functions cannot be used.\0";
		case ERROR_CANNOT_DETECT_DRIVER_FAILURE:
			return "Failure actions can only be set for Win32 services, not for drivers.\0";
		case ERROR_CANNOT_DETECT_PROCESS_ABORT:
			return "This service runs in the same process as the service control manager.Therefore, the service control manager cannot take action if this service's process terminates unexpectedly.\0";
		case ERROR_CANNOT_FIND_WND_CLASS:
			return "Cannot find window class.\0";
		case ERROR_CANNOT_IMPERSONATE:
			return "Unable to impersonate using a named pipe until data has been read from that pipe.\0";
		case ERROR_CANNOT_MAKE:
			return "The directory or file cannot be created.\0";
		case ERROR_CANNOT_OPEN_PROFILE:
			return "Unable to open the network connection profile.\0";
		case ERROR_CANT_ACCESS_DOMAIN_INFO:
			return "Indicates a Windows NT Server could not be contacted or that objects within the domain are protected such that necessary information could not be retrieved.\0";
		case ERROR_CANT_ACCESS_FILE:
			return "The file can not be accessed by the system.\0";
		case ERROR_CANT_DISABLE_MANDATORY:
			return "The group may not be disabled.\0";
		case ERROR_CANT_EVICT_ACTIVE_NODE:
			return "A cluster node cannot be evicted from the cluster while it is online.\0";
		case ERROR_CANT_OPEN_ANONYMOUS:
			return "Cannot open an anonymous level security token.\0";
		case ERROR_CANT_RESOLVE_FILENAME:
			return "The name of the file cannot be resolved by the system.\0";
		case ERROR_CANTOPEN:
			return "The configuration registry key could not be opened.\0";
		case ERROR_CANTREAD:
			return "The configuration registry key could not be read.\0";
		case ERROR_CANTWRITE:
			return "The configuration registry key could not be written.\0";
		case ERROR_CHILD_MUST_BE_VOLATILE:
			return "Cannot create a stable subkey under a volatile parent key.\0";
		case ERROR_CHILD_NOT_COMPLETE:
			return "The %1 application cannot be run in Win32 mode.\0";
		case ERROR_CHILD_WINDOW_MENU:
			return "Child windows cannot have menus.\0";
		case ERROR_CIRCULAR_DEPENDENCY:
			return "Circular service dependency was specified.\0";
		case ERROR_CLASS_ALREADY_EXISTS:
			return "Class already exists.\0";
		case ERROR_CLASS_DOES_NOT_EXIST:
			return "Class does not exist.\0";
		case ERROR_CLASS_HAS_WINDOWS:
			return "Class still has open windows.\0";
		case ERROR_CLIPBOARD_NOT_OPEN:
			return "Thread does not have a clipboard open.\0";
		case ERROR_CLIPPING_NOT_SUPPORTED:
			return "The requested clipping operation is not supported.\0";
		case ERROR_CLUSTER_SHUTTING_DOWN:
			return "The cluster software is shutting down.\0";
		case ERROR_CLUSTERLOG_CHKPOINT_NOT_FOUND:
			return "No checkpoint record was found in the cluster log.\0";
		case ERROR_CLUSTERLOG_CORRUPT:
			return "The cluster log is corrupt.\0";
		case ERROR_CLUSTERLOG_EXCEEDS_MAXSIZE:
			return "The cluster log exceeds its maximum size.\0";
		case ERROR_CLUSTERLOG_NOT_ENOUGH_SPACE:
			return "The minimum required disk space needed for logging is not available.\0";
		case ERROR_CLUSTERLOG_RECORD_EXCEEDS_MAXSIZE:
			return "The record could not be written to the cluster log since it exceeds the maximum size.\0";
		case ERROR_COMMITMENT_LIMIT:
			return "The paging file is too small for this operation to complete.\0";
		case ERROR_CONNECTED_OTHER_PASSWORD:
			return "The network connection was made successfully, but the user had to be prompted for a password other than the one originally specified.\0";
		case ERROR_CONNECTION_ABORTED:
			return "The network connection was aborted by the local system.\0";
		case ERROR_CONNECTION_ACTIVE:
			return "An invalid operation was attempted on an active network connection.\0";
		case ERROR_CONNECTION_COUNT_LIMIT:
			return "A connection to the server could not be made because the limit on the number of concurrent connections for this account has been reached.\0";
		case ERROR_CONNECTION_INVALID:
			return "An operation was attempted on a nonexistent network connection.\0";
		case ERROR_CONNECTION_REFUSED:
			return "The remote system refused the network connection.\0";
		case ERROR_CONNECTION_UNAVAIL:
			return "The device is not currently connected but it is a remembered connection.\0";
		case ERROR_CONTINUE:
			return "Continue with work in progress.\0";
		case ERROR_CONTROL_ID_NOT_FOUND:
			return "Control ID not found.\0";
		case ERROR_CORE_RESOURCE:
			return "The cluster resource could not be deleted since it is a core resource.\0";
		case ERROR_COUNTER_TIMEOUT:
			return "A serial I/O operation completed because the timeout period expired. The IOCTL_SERIAL_XOFF_COUNTER did not reach zero).\0";
		case ERROR_CRC:
			return "Data error (cyclic redundancy check).\0";
		case ERROR_CURRENT_DIRECTORY:
			return "The directory cannot be removed.\0";
		case ERROR_DATABASE_DOES_NOT_EXIST:
			return "The database specified does not exist.\0";
		case ERROR_DATABASE_FAILURE:
			return "Unable to read from or write to the database.\0";
		case ERROR_DATABASE_FULL:
			return "The database is full.\0";
		case ERROR_DC_NOT_FOUND:
			return "Invalid device context (DC) handle.\0";
		case ERROR_DDE_FAIL:
			return "An error occurred in sending the command to the application.\0";
		case ERROR_DECRYPTION_FAILED:
			return "The specified file could not be decrypted.\0";
		case ERROR_DELETING_ICM_XFORM:
			return "There was an error while deleting the color transform.\0";
		case ERROR_DEPENDENCY_ALREADY_EXISTS:
			return "The cluster resource cannot be made dependent on the specified resource because it is already dependent.\0";
		case ERROR_DEPENDENCY_NOT_FOUND:
			return "The cluster resource dependency cannot be found.\0";
		case ERROR_DEPENDENT_RESOURCE_EXISTS:
			return "The cluster resource cannot be moved to another group because other resources are dependent on it.\0";
		case ERROR_DEPENDENT_SERVICES_RUNNING:
			return "A stop control has been sent to a service that other running services are dependent on.\0";
		case ERROR_DESTINATION_ELEMENT_FULL:
			return "The indicated destination element already contains media.\0";
		case ERROR_DESTROY_OBJECT_OF_OTHER_THREAD:
			return "Cannot destroy object created by another thread.\0";
		case ERROR_DEV_NOT_EXIST:
			return "The specified network resource or device is no longer available.\0";
		case ERROR_DEVICE_ALREADY_REMEMBERED:
			return "An attempt was made to remember a device that had previously been remembered.\0";
		case ERROR_DEVICE_DOOR_OPEN:
			return "The device has indicated that its door is open.\0";
		case ERROR_DEVICE_IN_USE:
			return "The device is in use by an active process and cannot be disconnected.\0";
		case ERROR_DEVICE_NOT_AVAILABLE:
			return "The device is not ready for use.\0";
		case ERROR_DEVICE_NOT_CONNECTED:
			return "The device is not connected.\0";
		case ERROR_DEVICE_NOT_PARTITIONED:
			return "Tape partition information could not be found when loading a tape.\0";
		case ERROR_DEVICE_REINITIALIZATION_NEEDED:
			return "The indicated device requires reinitialization due to hardware errors.\0";
		case ERROR_DEVICE_REQUIRES_CLEANING:
			return "The device has indicated that cleaning is required before further operations are attempted.\0";
		case ERROR_DHCP_ADDRESS_CONFLICT:
			return "The DHCP client has obtained an IP address that is already in use on the network. The local interface will be disabled until the DHCP client can obtain a new address.\0";
		case ERROR_DIFFERENT_SERVICE_ACCOUNT:
			return "The account specified for this service is different from the account specified for other services running in the same process.\0";
		case ERROR_DIR_NOT_EMPTY:
			return "The directory is not empty.\0";
		case ERROR_DIR_NOT_ROOT:
			return "The directory is not a subdirectory of the root directory.\0";
		case ERROR_DIRECT_ACCESS_HANDLE:
			return "Attempt to use a file handle to an open disk partition for an operation other than raw disk I/O.\0";
		case ERROR_DIRECTORY:
			return "The directory name is invalid.\0";
		case ERROR_DISCARDED:
			return "The segment is already discarded and cannot be locked.\0";
		case ERROR_DISK_CHANGE:
			return "The program stopped because an alternate diskette was not inserted.\0";
		case ERROR_DISK_CORRUPT:
			return "The disk structure is corrupted and unreadable.\0";
		case ERROR_DISK_FULL:
			return "There is not enough space on the disk.\0";
		case ERROR_DISK_OPERATION_FAILED:
			return "While accessing the hard disk, a disk operation failed even after retries.\0";
		case ERROR_DISK_RECALIBRATE_FAILED:
			return "While accessing the hard disk, a recalibrate operation failed, even after retries.\0";
		case ERROR_DISK_RESET_FAILED:
			return "While accessing the hard disk, a disk controller reset was needed, but even that failed.\0";
		case ERROR_DLL_INIT_FAILED:
			return "A dynamic link library (DLL) initialization routine failed.\0";
		case ERROR_DLL_NOT_FOUND:
			return "One of the library files needed to run this application cannot be found.\0";
		case ERROR_DOMAIN_CONTROLLER_EXISTS:
			return "A domain controller with the specified name already exists.\0";
		case ERROR_DOMAIN_CONTROLLER_NOT_FOUND:
			return "Could not find the domain controller for this domain.\0"; 
		case ERROR_DOMAIN_EXISTS:
			return "The specified domain already exists.\0";
		case ERROR_DOMAIN_LIMIT_EXCEEDED:
			return "An attempt was made to exceed the limit on the number of domains per server.\0";
		case ERROR_DOMAIN_TRUST_INCONSISTENT:
			return "The name or security ID (SID) of the domain specified is inconsistent with the trust information for that domain.\0";
		case ERROR_DRIVE_LOCKED:
			return "The disk is in use or locked by another process.\0";
		case ERROR_DRIVE_MEDIA_MISMATCH:
			return "The drive and medium are not compatible r exist in different libraries.\0";
		case ERROR_DS_ATTRIBUTE_OR_VALUE_EXISTS:
			return "The specified directory service attribute or value already exists.\0";
		case ERROR_DS_ATTRIBUTE_TYPE_UNDEFINED:
			return "The attribute type specified to the directory service is not defined.\0";
		case ERROR_DS_BUSY:
			return "The directory service is busy.\0";
		case ERROR_DS_CANT_MOD_OBJ_CLASS:
			return "The directory service detected an attempt to modify the object class of an object.\0";
		case ERROR_DS_CANT_ON_NON_LEAF:
			return "The directory service can perform the requested operation only on a leaf object.\0";
		case ERROR_DS_CANT_ON_RDN:
			return "The directory service cannot perform the requested operation on the RDN attribute of an object.\0";
		case ERROR_DS_CROSS_DOM_MOVE_ERROR:
			return "The requested cross domain move operation could not be performed.\0";
		case ERROR_DS_GC_NOT_AVAILABLE:
			return "Unable to contact the global catalog server.\0";
		case ERROR_DS_INCORRECT_ROLE_OWNER:
			return "The requested operation could not be performed because the directory service is not the master for that type of operation.\0";
		case ERROR_DS_INVALID_ATTRIBUTE_SYNTAX:
			return "The attribute syntax specified to the directory service is invalid.\0";
		case ERROR_DS_MEMBERSHIP_EVALUATED_LOCALLY:
			return "The directory service evaluated group memberships locally.\0";
		case ERROR_DS_NO_ATTRIBUTE_OR_VALUE:
			return "The specified directory service attribute or value does not exist.\0";
		case ERROR_DS_NO_MORE_RIDS:
			return "The directory service has exhausted the pool of relative identifiers.\0";
		case ERROR_DS_NO_RIDS_ALLOCATED:
			return "The directory service was unable to allocate a relative identifier.\0";
		case ERROR_DS_NOT_INSTALLED:
			return "An error occurred while installing the Windows NT directory service. Please view the event log for more information.\0";
		case ERROR_DS_OBJ_CLASS_VIOLATION:
			return "The requested operation did not satisfy one or more constraints associated with the class of the object.\0"; 
		case ERROR_DS_RIDMGR_INIT_ERROR:
			return "The directory service was unable to initialize the subsystem that allocates relative identifiers.\0";
		case ERROR_DS_UNAVAILABLE:
			return "The directory service is unavailable.\0";
		case ERROR_DUP_DOMAINNAME:
			return "The workgroup or domain name is already in use by another computer on the network.\0";
		case ERROR_DUP_NAME:
			return "A duplicate name exists on the network.\0";
		case ERROR_DUPLICATE_SERVICE_NAME:
			return "The name is already in use as either a service name or a service display name.\0";
		case ERROR_DUPLICATE_TAG:
			return "The specified tag is already present.\0";
		case ERROR_DYNLINK_FROM_INVALID_RING:
			return "The operating system cannot run this application program.\0";
		case ERROR_EA_ACCESS_DENIED:
			return "Access to the extended attribute was denied.\0";
		case ERROR_EA_FILE_CORRUPT:
			return "The extended attribute file on the mounted file system is corrupt.\0";
		case ERROR_EA_LIST_INCONSISTENT:
			return "The extended attributes are inconsistent.\0";
		case ERROR_EA_TABLE_FULL:
			return "The extended attribute table file is full.\0";
		case ERROR_EAS_DIDNT_FIT:
			return "The extended attributes did not fit in the buffer.\0";
		case ERROR_EAS_NOT_SUPPORTED:
			return "The mounted file system does not support extended attributes.\0";
		case ERROR_EMPTY:
			return "The library, drive, or media pool is empty.\0";
		case ERROR_ENCRYPTION_FAILED:
			return "The specified file could not be encrypted.\0";
		case ERROR_END_OF_MEDIA:
			return "The physical end of the tape has been reached.\0";
		case ERROR_ENVVAR_NOT_FOUND:
			return "The system could not find the environment option that was entered.\0";
		case ERROR_EOM_OVERFLOW:
			return "Physical end of tape encountered.\0";
		case ERROR_EVENTLOG_CANT_START:
			return "No event log file could be opened, so the event logging service did not start.\0";
		case ERROR_EVENTLOG_FILE_CHANGED:
			return "The event log file has changed between read operations.\0";
		case ERROR_EVENTLOG_FILE_CORRUPT:
			return "The event log file is corrupted.\0";
		case ERROR_EXCEPTION_IN_SERVICE:
			return "An exception occurred in the service when handling the control request.\0";
		case ERROR_EXCL_SEM_ALREADY_OWNED:
			return "The exclusive semaphore is owned by another process.\0";
		case ERROR_EXE_MACHINE_TYPE_MISMATCH:
			return "The image file %1 is valid, but is for a machine type other than the current machine.\0";
		case ERROR_EXE_MARKED_INVALID:
			return "The operating system cannot run %1.\0";
		case ERROR_EXTENDED_ERROR:
			return "An extended error has occurred.\0";
		case ERROR_FAIL_I24:
			return "Fail on INT 24.\0";
		case ERROR_FAILED_SERVICE_CONTROLLER_CONNECT:
			return "The service process could not connect to the service controller.\0";
		case ERROR_FILE_CORRUPT:
			return "The file or directory is corrupted and unreadable.\0";
		case ERROR_FILE_ENCRYPTED:
			return "The specified file is encrypted and the user does not have the ability to decrypt it.\0";
		case ERROR_FILE_EXISTS:
			return "The file exists.\0";
		case ERROR_FILE_INVALID:
			return "The volume for a file has been externally altered so that the opened file is no longer valid.\0";
		case ERROR_FILE_NOT_ENCRYPTED:
			return "The specified file is not encrypted.\0";
		case ERROR_FILE_NOT_FOUND:
			return "The system cannot find the file specified.\0";
		case ERROR_FILE_OFFLINE:
			return "The remote storage service was not able to recall the file.\0";
		case ERROR_FILEMARK_DETECTED:
			return "A tape access reached a filemark.\0";
		case ERROR_FILENAME_EXCED_RANGE:
			return "The filename or extension is too long.\0";
		case ERROR_FLOPPY_BAD_REGISTERS:
			return "The floppy disk controller returned inconsistent results in its registers.\0";
		case ERROR_FLOPPY_ID_MARK_NOT_FOUND:
			return "No ID address mark was found on the floppy disk.\0";
		case ERROR_FLOPPY_UNKNOWN_ERROR:
			return "The floppy disk controller reported an error that is not recognized by the floppy disk driver.\0";
		case ERROR_FLOPPY_WRONG_CYLINDER:
			return "Mismatch between the floppy disk sector ID field and the floppy disk controller track address.\0";
		case ERROR_FULL_BACKUP:
			return "The backup failed. Check the directory to which you are backing the database.\0";
		case ERROR_FULLSCREEN_MODE:
			return "The requested operation cannot be performed in full-screen mode.\0";
		case ERROR_GEN_FAILURE:
			return "A device attached to the system is not functioning.\0";
		case ERROR_GENERIC_NOT_MAPPED:
			return "Generic access types were contained in an access mask which should already be mapped to nongeneric types.\0";
		case ERROR_GLOBAL_ONLY_HOOK:
			return "This hook procedure can only be set globally.\0";
		case ERROR_GRACEFUL_DISCONNECT:
			return "The network connection was gracefully closed.\0";
		case ERROR_GROUP_EXISTS:
			return "The specified group already exists.\0";
		case ERROR_GROUP_NOT_AVAILABLE:
			return "The cluster group is not available for any new requests.\0";
		case ERROR_GROUP_NOT_FOUND:
			return "The cluster group could not be found.\0";
		case ERROR_GROUP_NOT_ONLINE:
			return "The operation could not be completed because the cluster group is not online.\0";
		case ERROR_HANDLE_DISK_FULL:
			return "The disk is full.\0";
		case ERROR_HANDLE_EOF:
			return "Reached the end of the file.\0";
		case ERROR_HOOK_NEEDS_HMOD:
			return "Cannot set nonlocal hook without a module handle.\0";
		case ERROR_HOOK_NOT_INSTALLED:
			return "The hook procedure is not installed.\0";
		case ERROR_HOOK_TYPE_NOT_ALLOWED:
			return "Hook type not allowed.\0";
		case ERROR_HOST_NODE_NOT_AVAILABLE:
			return "A cluster node is not available for this operation.\0";
		case ERROR_HOST_NODE_NOT_GROUP_OWNER:
			return "The cluster node is not the owner of the group.\0";
		case ERROR_HOST_NODE_NOT_RESOURCE_OWNER:
			return "The cluster node is not the owner of the resource.\0";
		case ERROR_HOST_UNREACHABLE:
			return "The remote system is not reachable by the transport.\0";
		case ERROR_HOTKEY_ALREADY_REGISTERED:
			return "Hot key is already registered.\0";
		case ERROR_HOTKEY_NOT_REGISTERED:
			return "Hot key is not registered.\0";
		case ERROR_HWNDS_HAVE_DIFF_PARENT:
			return "All handles to windows in a multiple-window position structure must have the same parent.\0";
		case ERROR_ICM_NOT_ENABLED:
			return "Image Color Management is not enabled.\0";
		case ERROR_ILL_FORMED_PASSWORD:
			return "Unable to update the password. The value provided for the new password contains values that are not allowed in passwords.\0";
		case ERROR_ILLEGAL_ELEMENT_ADDRESS:
			return "The indicated element does not exist.\0";
		case ERROR_INC_BACKUP:
			return "The backup failed. Was a full backup done before?\0";
		case ERROR_INCORRECT_ADDRESS:
			return "The network address could not be used for the operation requested.\0";
		case ERROR_INDEX_ABSENT:
			return "Language not available.\0";
		case ERROR_INFLOOP_IN_RELOC_CHAIN:
			return "The operating system cannot run %1.\0";
		case ERROR_INSTALL_FAILURE:
			return "Fatal error during installation.\0";
		case ERROR_INSTALL_SERVICE:
			return "Failure accessing install service.\0";
		case ERROR_INSTALL_SOURCE_ABSENT:
			return "Install source unavailable.\0";
		case ERROR_INSTALL_SUSPEND:
			return "Installation suspended, incomplete.\0";
		case ERROR_INSTALL_USEREXIT:
			return "The user canceled the installation.\0";
		case ERROR_INSUFFICIENT_BUFFER:
			return "The data area passed to a system call is too small.\0";
		case ERROR_INTERNAL_DB_CORRUPTION:
			return "Unable to complete the requested operation because of either a catastrophic media failure or a data structure corruption on the disk.\0";
		case ERROR_INTERNAL_DB_ERROR:
			return "The local security authority database contains an internal inconsistency.\0";
		case ERROR_INTERNAL_ERROR:
			return "The security account database contains an internal inconsistency.\0";
		case ERROR_INVALID_ACCEL_HANDLE:
			return "Invalid accelerator table handle.\0";
		case ERROR_INVALID_ACCESS:
			return "The access code is invalid.\0";
		case ERROR_INVALID_ACCOUNT_NAME:
			return "The name provided is not a properly formed account name.\0";
		case ERROR_INVALID_ACL:
			return "The access control list (ACL) structure is invalid.\0";
		case ERROR_INVALID_ADDRESS:
			return "Attempt to access invalid address.\0";
		case ERROR_INVALID_AT_INTERRUPT_TIME:
			return "Cannot request exclusive semaphores at interrupt time.\0";
		case ERROR_INVALID_BLOCK:
			return "The storage control block address is invalid.\0";
		case ERROR_INVALID_BLOCK_LENGTH:
			return "When accessing a new tape of a multivolume partition, the current blocksize is incorrect.\0";
		case ERROR_INVALID_CATEGORY:
			return "The IOCTL call made by the application program is not correct.\0";
		case ERROR_INVALID_CLEANER:
			return "The media identifier does not represent a valid cleaner.\0";
		case ERROR_INVALID_CMM:
			return "The specified color management module is invalid.\0";
		case ERROR_INVALID_COLORSPACE:
			return "The specified color space is invalid.\0";
		case ERROR_INVALID_COMBOBOX_MESSAGE:
			return "Invalid message for a combo box because it does not have an edit control.\0";
		case ERROR_INVALID_COMPUTERNAME:
			return "The format of the specified computer name is invalid.\0";
		case ERROR_INVALID_CURSOR_HANDLE:
			return "Invalid cursor handle.\0";
		case ERROR_INVALID_DATA:
			return "The data is invalid.\0";
		case ERROR_INVALID_DATATYPE:
			return "The specified datatype is invalid.\0";
		case ERROR_INVALID_DLL:
			return "One of the library files needed to run this application is damaged.\0";
		case ERROR_INVALID_DOMAIN_ROLE:
			return "This operation is only allowed for the Primary Domain Controller of the domain.\0";
		case ERROR_INVALID_DOMAIN_STATE:
			return "The domain was in the wrong state to perform the security operation.\0";
		case ERROR_INVALID_DOMAINNAME:
			return "The format of the specified domain name is invalid.\0";
		case ERROR_INVALID_DRIVE:
			return "The system cannot find the drive specified.\0";
		case ERROR_INVALID_DWP_HANDLE:
			return "Invalid handle to a multiple-window position structure.\0";
		case ERROR_INVALID_EA_HANDLE:
			return "The specified extended attribute handle is invalid.\0";
		case ERROR_INVALID_EA_NAME:
			return "The specified extended attribute name was invalid.\0";
		case ERROR_INVALID_EDIT_HEIGHT:
			return "Height must be less than 256.\0";
		case ERROR_INVALID_ENVIRONMENT:
			return "The environment specified is invalid.\0";
		case ERROR_INVALID_EVENT_COUNT:
			return "The number of specified semaphore events for DosMuxSemWait is not correct.\0";
		case ERROR_INVALID_EVENTNAME:
			return "The format of the specified event name is invalid.\0";
		case ERROR_INVALID_EXE_SIGNATURE:
			return "Cannot run %1 in Win32 mode.\0";
		case ERROR_INVALID_FIELD:
			return "Record field does not exist.\0";
		case ERROR_INVALID_FILTER_PROC:
			return "Invalid hook procedure.\0";
		case ERROR_INVALID_FLAG_NUMBER:
			return "The flag passed is not correct.\0";
		case ERROR_INVALID_FLAGS:
			return "Invalid flags.\0";
		case ERROR_INVALID_FORM_NAME:
			return "The specified form name is invalid.\0";
		case ERROR_INVALID_FORM_SIZE:
			return "The specified form size is invalid.\0";
		case ERROR_INVALID_FUNCTION:
			return "Incorrect function.\0";
		case ERROR_INVALID_GROUP_ATTRIBUTES:
			return "The specified attributes are invalid, or incompatible with the attributes for the group as a whole.\0";
		case ERROR_INVALID_GROUPNAME:
			return "The format of the specified group name is invalid.\0";
		case ERROR_INVALID_GW_COMMAND:
			return "Invalid GW_* command.\0";
		case ERROR_INVALID_HANDLE:
			return "The handle is invalid.\0";
		case ERROR_INVALID_HANDLE_STATE:
			return "Handle is in an invalid state.\0";
		case ERROR_INVALID_HOOK_FILTER:
			return "Invalid hook procedure type.\0";
		case ERROR_INVALID_HOOK_HANDLE:
			return "Invalid hook handle.\0";
		case ERROR_INVALID_ICON_HANDLE:
			return "Invalid icon handle.\0";
		case ERROR_INVALID_ID_AUTHORITY:
			return "The value provided was an invalid value for an identifier authority.\0";
		case ERROR_INVALID_INDEX:
			return "Invalid index.\0";
		case ERROR_INVALID_KEYBOARD_HANDLE:
			return "Invalid keyboard layout handle.\0";
		case ERROR_INVALID_LB_MESSAGE:
			return "Invalid message for single-selection list box.\0";
		case ERROR_INVALID_LEVEL:
			return "The system call level is not correct.\0";
		case ERROR_INVALID_LIBRARY:
			return "The library identifier does not represent a valid library.\0";
		case ERROR_INVALID_LIST_FORMAT:
			return "The DosMuxSemWait list is not correct.\0";
		case ERROR_INVALID_LOGON_HOURS:
			return "Logon failure: account logon time restriction violation.\0";
		case ERROR_INVALID_LOGON_TYPE:
			return "A logon request contained an invalid logon type value.\0";
		case ERROR_INVALID_MEDIA:
			return "The media identifier does not represent a valid medium.\0";
		case ERROR_INVALID_MEDIA_POOL:
			return "The media pool identifier does not represent a valid media pool.\0";
		case ERROR_INVALID_MEMBER:
			return "A new member could not be added to a local group because the member has the wrong account type.\0";
		case ERROR_INVALID_MENU_HANDLE:
			return "Invalid menu handle.\0";
		case ERROR_INVALID_MESSAGE:
			return "The window cannot act on the sent message.\0";
		case ERROR_INVALID_MESSAGEDEST:
			return "The format of the specified message destination is invalid.\0";
		case ERROR_INVALID_MESSAGENAME:
			return "The format of the specified message name is invalid.\0";
		case ERROR_INVALID_MINALLOCSIZE:
			return "The operating system cannot run %1.\0";
		case ERROR_INVALID_MODULETYPE:
			return "The operating system cannot run %1.\0";
		case ERROR_INVALID_MONITOR_HANDLE:
			return "Invalid monitor handle.\0";
		case ERROR_INVALID_MSGBOX_STYLE:
			return "Invalid message box style.\0";
		case ERROR_INVALID_NAME:
			return "The filename, directory name, or volume label syntax is incorrect.\0";
		case ERROR_INVALID_NETNAME:
			return "The format of the specified network name is invalid.\0";
		case ERROR_INVALID_OPERATION:
			return "The operation identifier is not valid.\0";
		case ERROR_INVALID_OPLOCK_PROTOCOL:
			return "An invalid oplock acknowledgment was received by the system.\0";
		case ERROR_INVALID_ORDINAL:
			return "The operating system cannot run %1.\0";
		case ERROR_INVALID_OWNER:
			return "This security ID may not be assigned as the owner of this object.\0";
		case ERROR_INVALID_PARAMETER:
			return "The parameter is incorrect.\0";
		case ERROR_INVALID_PASSWORD:
			return "The specified network password is not correct.\0";
		case ERROR_INVALID_PASSWORDNAME:
			return "The format of the specified password is invalid.\0";
		case ERROR_INVALID_PIXEL_FORMAT:
			return "The pixel format is invalid.\0";
		case ERROR_INVALID_PRIMARY_GROUP:
			return "This security ID may not be assigned as the primary group of an object.\0";
		case ERROR_INVALID_PRINT_MONITOR:
			return "The specified print monitor does not have the required functions.\0";
		case ERROR_INVALID_PRINTER_COMMAND:
			return "The printer command is invalid.\0";
		case ERROR_INVALID_PRINTER_NAME:
			return "The printer name is invalid.\0";
		case ERROR_INVALID_PRINTER_STATE:
			return "The state of the printer is invalid.\0";
		case ERROR_INVALID_PRIORITY:
			return "The specified priority is invalid.\0";
		case ERROR_INVALID_PROFILE:
			return "The specified color profile is invalid.\0";
		case ERROR_INVALID_SCROLLBAR_RANGE:
			return "Scroll bar range cannot be greater than 0x7FFF.\0";
		case ERROR_INVALID_SECURITY_DESCR:
			return "The security descriptor structure is invalid.\0";
		case ERROR_INVALID_SEGDPL:
			return "The operating system cannot run %1.\0";
		case ERROR_INVALID_SEGMENT_NUMBER:
			return "The system detected a segment number that was not correct.\0";
		case ERROR_INVALID_SEPARATOR_FILE:
			return "The specified separator file is invalid.\0";
		case ERROR_INVALID_SERVER_STATE:
			return "The security account manager (SAM) or local security authority (LSA) server was in the wrong state to perform the security operation.\0";
		case ERROR_INVALID_SERVICE_ACCOUNT:
			return "The account name is invalid or does not exist.\0";
		case ERROR_INVALID_SERVICE_CONTROL:
			return "The requested control is not valid for this service.\0";
		case ERROR_INVALID_SERVICE_LOCK:
			return "The specified service database lock is invalid.\0";
		case ERROR_INVALID_SERVICENAME:
			return "The format of the specified service name is invalid.\0";
		case ERROR_INVALID_SHARENAME:
			return "The format of the specified share name is invalid.\0";
		case ERROR_INVALID_SHOWWIN_COMMAND:
			return "Cannot show or remove the window in the way specified.\0";
		case ERROR_INVALID_SID:
			return "The security ID structure is invalid.\0";
		case ERROR_INVALID_SIGNAL_NUMBER:
			return "The signal being posted is not correct.\0";
		case ERROR_INVALID_SPI_VALUE:
			return "Invalid system-wide (SPI_*) parameter.\0";
		case ERROR_INVALID_STACKSEG:
			return "The operating system cannot run %1.\0";
		case ERROR_INVALID_STARTING_CODESEG:
			return "The operating system cannot run %1.\0";
		case ERROR_INVALID_STATE:
			return "The group or resource is not in the correct state to perform the requested operation.\0";
		case ERROR_INVALID_SUB_AUTHORITY:
			return "The subauthority part of a security ID is invalid for this particular use.\0";
		case ERROR_INVALID_TARGET_HANDLE:
			return "The target internal file identifier is incorrect.\0";
		case ERROR_INVALID_THREAD_ID:
			return "Invalid thread identifier.\0";
		case ERROR_INVALID_TIME:
			return "The specified time is invalid.\0";
		case ERROR_INVALID_TRANSFORM:
			return "The specified color transform is invalid.\0";
		case ERROR_INVALID_USER_BUFFER:
			return "The supplied user buffer is not valid for the requested operation.\0";
		case ERROR_INVALID_VERIFY_SWITCH:
			return "The verify-on-write switch parameter value is not correct.\0";
		case ERROR_INVALID_WINDOW_HANDLE:
			return "Invalid window handle.\0";
		case ERROR_INVALID_WINDOW_STYLE:
			return "The window style or class attribute is invalid for this operation.\0";
		case ERROR_INVALID_WORKSTATION:
			return "Logon failure: user not allowed to log on to this computer.\0";
		case ERROR_IO_DEVICE:
			return "The request could not be performed because of an I/O device error.\0";
		case ERROR_IO_INCOMPLETE:
			return "Overlapped I/O event is not in a signaled state.\0";
		case ERROR_IO_PENDING:
			return "Overlapped I/O operation is in progress.\0";
		case ERROR_IOPL_NOT_ENABLED:
			return "The operating system is not presently configured to run this application.\0";
		case ERROR_IRQ_BUSY:
			return "Unable to open a device that was sharing an interrupt request (IRQ) with other devices. At least one other device that uses that IRQ was already opened.\0";
		case ERROR_IS_JOIN_PATH:
			return "Not enough resources are available to process this command.\0";
		case ERROR_IS_JOIN_TARGET:
			return "A JOIN or SUBST command cannot be used for a drive that contains previously joined drives.\0";
		case ERROR_IS_JOINED:
			return "An attempt was made to use a JOIN or SUBST command on a drive that has already been joined.\0";
		case ERROR_IS_SUBST_PATH:
			return "The path specified is being used in a substitute.\0";
		case ERROR_IS_SUBST_TARGET:
			return "An attempt was made to join or substitute a drive for which a directory on the drive is the target of a previous substitute.\0";
		case ERROR_IS_SUBSTED:
			return "An attempt was made to use a JOIN or SUBST command on a drive that has already been substituted.\0";
		case ERROR_ITERATED_DATA_EXCEEDS_64k:
			return "The operating system cannot run %1.\0";
		case ERROR_JOIN_TO_JOIN:
			return "The system tried to join a drive to a directory on a joined drive.\0";
		case ERROR_JOIN_TO_SUBST:
			return "The system tried to join a drive to a directory on a substituted drive.\0";
		case ERROR_JOURNAL_HOOK_SET:
			return "The journal hook procedure is already installed.\0";
		case ERROR_KEY_DELETED:
			return "Illegal operation attempted on a registry key that has been marked for deletion.\0";
		case ERROR_KEY_HAS_CHILDREN:
			return "Cannot create a symbolic link in a registry key that already has subkeys or values.\0";
		case ERROR_LABEL_TOO_LONG:
			return "The volume label you entered exceeds the label character limit of the target file system.\0";
		case ERROR_LAST_ADMIN:
			return "The last remaining administration account cannot be disabled or deleted.\0";
		case ERROR_LB_WITHOUT_TABSTOPS:
			return "This list box does not support tab stops.\0";
		case ERROR_LIBRARY_OFFLINE:
			return "The operation cannot be performed on an offline library.\0";
		case ERROR_LICENSE_QUOTA_EXCEEDED:
			return "The service being accessed is licensed for a particular number of connections. No more connections can be made to the service at this time because there are already as many connections as the service can accept.\0";
		case ERROR_LISTBOX_ID_NOT_FOUND:
			return "The list box identifier was not found.\0";
		case ERROR_LM_CROSS_ENCRYPTION_REQUIRED:
			return "A cross-encrypted password is necessary to change this user password.\0";
		case ERROR_LOCAL_USER_SESSION_KEY:
			return "No encryption key is available. A well-known encryption key was returned.\0";
		case ERROR_LOCK_FAILED:
			return "Unable to lock a region of a file.\0";
		case ERROR_LOCK_VIOLATION:
			return "The process cannot access the file because another process has locked a portion of the file.\0";
		case ERROR_LOCKED:
			return "The segment is locked and cannot be reallocated.\0";
		case ERROR_LOG_FILE_FULL:
			return "The event log file is full.\0";
		case ERROR_LOGIN_TIME_RESTRICTION:
			return "Attempting to log in during an unauthorized time of day for this account.\0";
		case ERROR_LOGIN_WKSTA_RESTRICTION:
			return "The account is not authorized to log in from this station.\0";
		case ERROR_LOGON_FAILURE:
			return "Logon failure: unknown user name or bad password.\0";
		case ERROR_LOGON_NOT_GRANTED:
			return "Logon failure: the user has not been granted the requested logon type at this computer.\0";
		case ERROR_LOGON_SESSION_COLLISION:
			return "The logon session ID is already in use.\0";
		case ERROR_LOGON_SESSION_EXISTS:
			return "Cannot start a new logon session with an ID that is already in use.\0";
		case ERROR_LOGON_TYPE_NOT_GRANTED:
			return "Logon failure: the user has not been granted the requested logon type at this computer.\0";
		case ERROR_LUIDS_EXHAUSTED:
			return "No more local user identifiers (LUIDs) are available.\0";
		case ERROR_MAGAZINE_NOT_PRESENT:
			return "The indicated element is part of a magazine that is not present.\0";
		case ERROR_MAPPED_ALIGNMENT:
			return "The base address or the file offset specified does not have the proper alignment.\0";
		case ERROR_MAX_THRDS_REACHED:
			return "No more threads can be created in the system.\0";
		case ERROR_MEDIA_CHANGED:
			return "The media in the drive may have changed.\0";
		case ERROR_MEDIA_INCOMPATIBLE:
			return "The medium is not compatible with the device or media pool.\0";
		case ERROR_MEDIA_NOT_AVAILABLE:
			return "The media is not mounted or ready for use.\0";
		case ERROR_MEDIA_OFFLINE:
			return "The medium currently exists in an offline library and must be online to perform this operation.\0";
		case ERROR_MEDIA_UNAVAILABLE:
			return "No media is currently available in this media pool or library.\0";
		case ERROR_MEMBER_IN_ALIAS:
			return "The specified account name is already a member of the local group.\0";
		case ERROR_MEMBER_IN_GROUP:
			return "Either the specified user account is already a member of the specified group, or the specified group cannot be deleted because it contains a member.\0";
		case ERROR_MEMBER_NOT_IN_ALIAS:
			return "The specified account name is not a member of the local group.\0";
		case ERROR_MEMBER_NOT_IN_GROUP:
			return "The specified user account is not a member of the specified group account.\0";
		case ERROR_MEMBERS_PRIMARY_GROUP:
			return "The user cannot be removed from a group because the group is currently the user's primary group.\0";
		case ERROR_MENU_ITEM_NOT_FOUND:
			return "A menu item was not found.\0";
		case ERROR_MESSAGE_SYNC_ONLY:
			return "The message can be used only with synchronous operations.\0";
		case ERROR_META_EXPANSION_TOO_LONG:
			return "The global filename characters, * or ?, are entered incorrectly or too many global filename characters are specified.\0";
		case ERROR_METAFILE_NOT_SUPPORTED:
			return "The requested metafile operation is not supported.\0";
		case ERROR_MOD_NOT_FOUND:
			return "The specified module could not be found.\0";
		case ERROR_MORE_DATA:
			return "More data is available.\0";
		case ERROR_MORE_WRITES:
			return "A serial I/O operation was completed by another write to the serial port. The IOCTL_SERIAL_XOFF_COUNTER reached zero.\0";
		case ERROR_MR_MID_NOT_FOUND:
			return "The system cannot find message text for message number 0x%1 in the message file for %2.\0";
		case ERROR_NEGATIVE_SEEK:
			return "An attempt was made to move the file pointer before the beginning of the file.\0";
		case ERROR_NESTING_NOT_ALLOWED:
			return "Can't nest calls to LoadModule.\0";
		case ERROR_NET_WRITE_FAULT:
			return "A write fault occurred on the network.\0";
		case ERROR_NETLOGON_NOT_STARTED:
			return "An attempt was made to logon, but the network logon service was not started.\0";
		case ERROR_NETNAME_DELETED:
			return "The specified network name is no longer available.\0";
		case ERROR_NETWORK_ACCESS_DENIED:
			return "Network access is denied.\0";
		case ERROR_NETWORK_BUSY:
			return "The network is busy.\0";
		case ERROR_NETWORK_UNREACHABLE:
			return "The remote network is not reachable by the transport.\0";
		case ERROR_NO_ASSOCIATION:
			return "No application is associated with the specified file for this operation.\0";
		case ERROR_NO_BROWSER_SERVERS_FOUND:
			return "The list of servers for this workgroup is not currently available.\0";
		case ERROR_NO_DATA:
			return "The pipe is being closed.\0";
		case ERROR_NO_DATA_DETECTED:
			return "No more data is on the tape.\0";
		case ERROR_NO_EFS:
			return "The required encryption driver is not loaded for this system.\0";
		case ERROR_NO_IMPERSONATION_TOKEN:
			return "An attempt has been made to operate on an impersonation token by a thread that is not currently impersonating a client.\0";
		case ERROR_NO_INHERITANCE:
			return "Indicates an ACL contains no inheritable components.\0";
		case ERROR_NO_LOG_SPACE:
			return "System could not allocate the required space in a registry log.\0";
		case ERROR_NO_LOGON_SERVERS:
			return "There are currently no logon servers available to service the logon request.\0";
		case ERROR_NO_MATCH:
			return "There was no match for the specified key in the index.\0";
		case ERROR_NO_MEDIA_IN_DRIVE:
			return "No media in drive.\0";
		case ERROR_NO_MORE_DEVICES:
			return "No more local devices.\0";
		case ERROR_NO_MORE_FILES:
			return "There are no more files.\0";
		case ERROR_NO_MORE_ITEMS:
			return "No more data is available.\0";
		case ERROR_NO_MORE_SEARCH_HANDLES:
			return "No more internal file identifiers available.\0";
		case ERROR_NO_MORE_USER_HANDLES:
			return "The current process has used all of its system allowance of handles for Window Manager objects.\0";
		case ERROR_NO_NET_OR_BAD_PATH:
			return "No network provider accepted the given network path.\0";
		case ERROR_NO_NETWORK:
			return "The network is not present or not started.\0";
		case ERROR_NO_PROC_SLOTS:
			return "The system cannot start another process at this time.\0";
		case ERROR_NO_QUOTAS_FOR_ACCOUNT:
			return "No system quota limits are specifically set for this account.\0";
		case ERROR_NO_RECOVERY_POLICY:
			return "There is no encryption recovery policy configured for this system.\0";
		case ERROR_NO_RECOVERY_PROGRAM:
			return "No recovery program has been configured for this service.\0";
		case ERROR_NO_SCROLLBARS:
			return "The window does not have scroll bars.\0";
		case ERROR_NO_SECURITY_ON_OBJECT:
			return "Unable to perform a security operation on an object that has no associated security.\0";
		case ERROR_NO_SHUTDOWN_IN_PROGRESS:
			return "Unable to abort the system shutdown because no shutdown was in progress.\0";
		case ERROR_NO_SIGNAL_SENT:
			return "No process in the command subtree has a signal handler.\0";
		case ERROR_NO_SITENAME:
			return "No site name is available for this machine.\0";
		case ERROR_NO_SPOOL_SPACE:
			return "Space to store the file waiting to be printed is not available on the server.\0";
		case ERROR_NO_SUCH_ALIAS:
			return "The specified local group does not exist.\0";
		case ERROR_NO_SUCH_DOMAIN: 
			return "The specified domain did not exist.\0";
		case ERROR_NO_SUCH_GROUP:
			return "The specified group does not exist.\0";
		case ERROR_NO_SUCH_LOGON_SESSION:
			return "A specified logon session does not exist. It may already have been terminated.\0";
		case ERROR_NO_SUCH_MEMBER:
			return "A new member could not be added to a local group because the member does not exist.\0";
		case ERROR_NO_SUCH_PACKAGE:
			return "A specified authentication package is unknown.\0";
		case ERROR_NO_SUCH_PRIVILEGE:
			return "A specified privilege does not exist.\0";
		case ERROR_NO_SUCH_SITE:
			return "The specified site does not exist.\0";
		case ERROR_NO_SUCH_USER:
			return "The specified user does not exist.\0";
		case ERROR_NO_SYSTEM_MENU:
			return "The window does not have a system menu.\0";
		case ERROR_NO_SYSTEM_RESOURCES:
			return "Insufficient system resources exist to complete the requested service.\0";
		case ERROR_NO_TOKEN:
			return "An attempt was made to reference a token that does not exist.\0";
		case ERROR_NO_TRACKING_SERVICE:
			return "The tracking (workstation) service is not running.\0";
		case ERROR_NO_TRUST_LSA_SECRET:
			return "The workstation does not have a trust secret.\0";
		case ERROR_NO_TRUST_SAM_ACCOUNT:
			return "The SAM database on the Windows NT Server does not have a computer account for this workstation trust relationship.\0";
		case ERROR_NO_UNICODE_TRANSLATION:
			return "No mapping for the Unicode character exists in the target multi-byte code page.\0";
		case ERROR_NO_USER_KEYS:
			return "There are no EFS keys defined for the user.\0";
		case ERROR_NO_USER_SESSION_KEY:
			return "There is no user session key for the specified logon session.\0";
		case ERROR_NO_VOLUME_ID:
			return "The Volume ID could not be found.\0";
		case ERROR_NO_VOLUME_LABEL:
			return "The disk has no volume label.\0";
		case ERROR_NO_WILDCARD_CHARACTERS:
			return "No wildcards were found.\0";
		case ERROR_NOACCESS:
			return "Invalid access to memory location.\0";
		case ERROR_NOLOGON_INTERDOMAIN_TRUST_ACCOUNT:
			return "The account used is an interdomain trust account. Use your global user account or local user account to access this server.\0";
		case ERROR_NOLOGON_SERVER_TRUST_ACCOUNT:
			return "The account used is a server trust account. Use your global user account or local user account to access this server.\0";
		case ERROR_NOLOGON_WORKSTATION_TRUST_ACCOUNT:
			return "The account used is a computer account. Use your global user account or local user account to access this server.\0";
		case ERROR_NON_MDICHILD_WINDOW:
			return "Cannot process a message from a window that is not a multiple document interface (MDI) window.\0";
		case ERROR_NONE_MAPPED:
			return "No mapping between account names and security IDs was done.\0";
		case ERROR_NONPAGED_SYSTEM_RESOURCES:
			return "Insufficient system resources exist to complete the requested service.\0";
		case ERROR_NOT_A_REPARSE_POINT:
			return "The file or directory is not a reparse point.\0";
		case ERROR_NOT_ALL_ASSIGNED:
			return "Not all privileges referenced are assigned to the caller.\0";
		case ERROR_NOT_AUTHENTICATED:
			return "The operation being requested was not performed because the user has not been authenticated.\0";
		case ERROR_NOT_CHILD_WINDOW:
			return "The window is not a child window.\0";
		case ERROR_NOT_CONNECTED:
			return "This network connection does not exist.\0";
		case ERROR_NOT_CONTAINER:
			return "Cannot enumerate a noncontainer.\0";
		case ERROR_NOT_DOS_DISK:
			return "The specified disk or diskette cannot be accessed.\0";
		case ERROR_NOT_EMPTY:
			return "The library, drive, or media pool must be empty to perform this operation.\0";
		case ERROR_NOT_ENOUGH_MEMORY:
			return "Not enough storage is available to process this command.\0";
		case ERROR_NOT_ENOUGH_QUOTA:
			return "Not enough quota is available to process this command.\0";
		case ERROR_NOT_ENOUGH_SERVER_MEMORY:
			return "Not enough server storage is available to process this command.\0";
		case ERROR_NOT_EXPORT_FORMAT:
			return "The specified file is not in the defined EFS export format.\0";
		case ERROR_NOT_FOUND:
			return "Element not found.\0";
		case ERROR_NOT_JOINED:
			return "The system tried to delete the JOIN of a drive that is not joined.\0";
		case ERROR_NOT_LOCKED:
			return "The segment is already unlocked.\0";
		case ERROR_NOT_LOGGED_ON:
			return "The operation being requested was not performed because the user has not logged on to the network. The specified service does not exist.\0";
		case ERROR_NOT_LOGON_PROCESS:
			return "The requested action is restricted for use by logon processes only. The calling process has not registered as a logon process.\0";
		case ERROR_NOT_OWNER:
			return "Attempt to release mutex not owned by caller.\0";
		case ERROR_NOT_QUORUM_CAPABLE:
			return "The cluster could not make the specified resource a quorum resource because it is not capable of being a quorum resource.\0";
		case ERROR_NOT_QUORUM_CLASS:
			return "The cluster could not make the specified resource a quorum resource because it does not belong to a shared storage class.\0";
		case ERROR_NOT_READY:
			return "The device is not ready.\0";
		case ERROR_NOT_REGISTRY_FILE:
			return "The system has attempted to load or restore a file into the registry, but the specified file is not in a registry file format.\0";
		case ERROR_NOT_SAME_DEVICE:
			return "The system cannot move the file to a different disk drive.\0";
		case ERROR_NOT_SUBSTED:
			return "The system tried to delete the substitution of a drive that is not substituted.\0";
		case ERROR_NOT_SUPPORTED:
			return "The network request is not supported.\0";
		case ERROR_NOTIFY_ENUM_DIR:
			return "A notify change request is being completed and the information is not being returned in the caller's buffer. The caller now needs to enumerate the files to find the changes.\0";
		case ERROR_NT_CROSS_ENCRYPTION_REQUIRED:
			return "A cross-encrypted password is necessary to change a user password.\0";
		case ERROR_NULL_LM_PASSWORD:
			return "The Windows NT password is too complex to be converted to a LAN Manager password. The LAN Manager password returned is a NULL string.\0";
		case ERROR_OBJECT_ALREADY_EXISTS:
			return "The object already exists.\0";
		case ERROR_OBJECT_IN_LIST:
			return "The object is already in the list.\0";
		case ERROR_OBJECT_NOT_FOUND:
			return "The object identifier does not represent a valid object.\0";
		case ERROR_OLD_WIN_VERSION:
			return "The specified program requires a newer version of Windows.\0";
		case ERROR_OPEN_FAILED:
			return "The system cannot open the device or file specified.\0";
		case ERROR_OPEN_FILES:
			return "This network connection has files open or requests pending.\0";
		case ERROR_OPERATION_ABORTED:
			return "The I/O operation has been aborted because of either a thread exit or an application request.\0";
		case ERROR_OPLOCK_NOT_GRANTED:
			return "The oplock request is denied.\0";
		case ERROR_OUT_OF_PAPER:
			return "The printer is out of paper.\0";
		case ERROR_OUT_OF_STRUCTURES:
			return "Storage to process this request is not available.\0";
		case ERROR_OUTOFMEMORY:
			return "Not enough storage is available to complete this operation.\0";
		case ERROR_PAGED_SYSTEM_RESOURCES:
			return "Insufficient system resources exist to complete the requested service.\0";
		case ERROR_PAGEFILE_QUOTA:
			return "Insufficient quota to complete the requested service.\0";
		case ERROR_PARTIAL_COPY:
			return "Only part of a ReadProcessMemoty or WriteProcessMemory request was completed.\0";
		case ERROR_PARTITION_FAILURE:
			return "Tape could not be partitioned.\0";
		case ERROR_PASSWORD_EXPIRED:
			return "Logon failure: the specified account password has expired.\0";
		case ERROR_PASSWORD_MUST_CHANGE:
			return "The user must change his password before he logs on the first time.\0";
		case ERROR_PASSWORD_RESTRICTION:
			return "Unable to update the password because a password update rule has been violated.\0";
		case ERROR_PATH_BUSY:
			return "The path specified cannot be used at this time.\0";
		case ERROR_PATH_NOT_FOUND:
			return "The system cannot find the path specified.\0";
		case ERROR_PIPE_BUSY:
			return "All pipe instances are busy.\0";
		case ERROR_PIPE_CONNECTED:
			return "There is a process on other end of the pipe.\0";
		case ERROR_PIPE_LISTENING:
			return "Waiting for a process to open the other end of the pipe.\0";
		case ERROR_PIPE_NOT_CONNECTED:
			return "No process is on the other end of the pipe.\0";
		case ERROR_POINT_NOT_FOUND:
			return "The point passed to GetMouseMovePoints is not in the buffer.\0";
		case ERROR_POPUP_ALREADY_ACTIVE:
			return "Popup menu already active.\0";
		case ERROR_PORT_UNREACHABLE:
			return "No service is operating at the destination network endpoint on the remote system.\0";
		case ERROR_POSSIBLE_DEADLOCK:
			return "A potential deadlock condition has been detected.\0";
		case ERROR_PRINT_CANCELLED:
			return "Your file waiting to be printed was deleted.\0";
		case ERROR_PRINT_MONITOR_ALREADY_INSTALLED:
			return "The specified print monitor has already been installed.\0";
		case ERROR_PRINT_MONITOR_IN_USE:
			return "The specified print monitor is currently in use.\0";
		case ERROR_PRINT_PROCESSOR_ALREADY_INSTALLED:
			return "The specified print processor has already been installed.\0";
		case ERROR_PRINTER_ALREADY_EXISTS:
			return "The printer already exists.\0";
		case ERROR_PRINTER_DELETED:
			return "The specified printer has been deleted.\0";
		case ERROR_PRINTER_DRIVER_ALREADY_INSTALLED:
			return "The specified printer driver is already installed.\0";
		case ERROR_PRINTER_DRIVER_IN_USE:
			return "The specified printer driver is currently in use.\0";
		case ERROR_PRINTER_HAS_JOBS_QUEUED:
			return "The requested operation is not allowed when there are jobs queued to the printer.\0";
		case ERROR_PRINTQ_FULL:
			return "The printer queue is full.\0";
		case ERROR_PRIVATE_DIALOG_INDEX:
			return "Using private DIALOG window words.\0";
		case ERROR_PRIVILEGE_NOT_HELD:
			return "A required privilege is not held by the client.\0";
		case ERROR_PROC_NOT_FOUND:
			return "The specified procedure could not be found.\0";
		case ERROR_PROCESS_ABORTED:
			return "The process terminated unexpectedly.\0";
		case ERROR_PRODUCT_UNINSTALLED:
			return "Product is uninstalled.\0";
		case ERROR_PROFILE_NOT_ASSOCIATED_WITH_DEVICE:
			return "The specified color profile is not associated with any device.\0";
		case ERROR_PROFILE_NOT_FOUND:
			return "The specified color profile was not found.\0";
		case ERROR_PROTOCOL_UNREACHABLE:
			return "The remote system does not support the transport protocol.\0";
		case ERROR_QUORUM_RESOURCE:
			return "The cluster resource could not be deleted or brought offline because it is the quorum resource.\0";
		case ERROR_QUORUM_RESOURCE_ONLINE_FAILED:
			return "The quorum resource failed to come online.\0";
		case ERROR_QUORUMLOG_OPEN_FAILED:
			return "The quorum log could not be created or mounted successfully.\0";
		case ERROR_READ_FAULT:
			return "The system cannot read from the specified device.\0";
		case ERROR_REC_NON_EXISTENT:
			return "The name does not exist in the WINS database.\0";
		case ERROR_REDIR_PAUSED:
			return "The specified printer or disk device has been paused.\0";
		case ERROR_REDIRECTOR_HAS_OPEN_HANDLES:
			return "The redirector is in use and cannot be unloaded.\0";
		case ERROR_REGISTRY_CORRUPT:
			return "The registry is corrupted. The structure of one of the files that contains registry data is corrupted, or the system's image of the file in memory is corrupted, or the file could not be recovered because the alternate copy or log was absent or corrupted.\0";
		case ERROR_REGISTRY_IO_FAILED:
			return "An I/O operation initiated by the registry failed unrecoverably. The registry could not read in, or write out, or flush, one of the files that contain the system's image of the registry.\0";
		case ERROR_REGISTRY_RECOVERED:
			return "One of the files in the registry database had to be recovered by use of a log or alternate copy. The recovery was successful.\0";
		case ERROR_RELOC_CHAIN_XEEDS_SEGLIM:
			return "The operating system cannot run %1.\0";
		case ERROR_REM_NOT_LIST:
			return "The remote computer is not available.\0";
		case ERROR_REMOTE_SESSION_LIMIT_EXCEEDED:
			return "An attempt was made to establish a session to a network server, but there are already too many sessions established to that server.\0";
		case ERROR_REMOTE_STORAGE_MEDIA_ERROR:
			return "The remote storage service encountered a media error.\0";
		case ERROR_REMOTE_STORAGE_NOT_ACTIVE:
			return "The remote storage service is not operational at this time.\0";
		case ERROR_REPARSE_ATTRIBUTE_CONFLICT:
			return "The reparse point attribute cannot be set because it conflicts with an existing attribute.\0";
		case ERROR_REQ_NOT_ACCEP:
			return "No more connections can be made to this remote computer at this time because there are already as many connections as the computer can accept.\0";
		case ERROR_REQUEST_ABORTED:
			return "The request was aborted.\0";
		case ERROR_REQUEST_REFUSED:
			return "The operator or administrator has refused the request.\0";
		case ERROR_REQUIRES_INTERACTIVE_WINDOWSTATION:
			return "This operation requires an interactive window station.\0";
		case ERROR_RESMON_CREATE_FAILED:
			return "The cluster resource could not be created in the specified resource monitor.\0";
		case ERROR_RESMON_ONLINE_FAILED:
			return "The cluster resource could not be brought online by the resource monitor.\0";
		case ERROR_RESOURCE_DATA_NOT_FOUND:
			return "The specified image file did not contain a resource section.\0";
		case ERROR_RESOURCE_DISABLED:
			return "A resource required for this operation is disabled.\0";
		case ERROR_RESOURCE_LANG_NOT_FOUND:
			return "The specified resource language ID cannot be found in the image file.\0";
		case ERROR_RESOURCE_NAME_NOT_FOUND:
			return "The specified resource name cannot be found in the image file.\0";
		case ERROR_RESOURCE_NOT_AVAILABLE:
			return "The cluster resource is not available.\0";
		case ERROR_RESOURCE_NOT_FOUND:
			return "The cluster resource could not be found.\0";
		case ERROR_RESOURCE_NOT_ONLINE:
			return "The cluster resource is not online.\0";
		case ERROR_RESOURCE_NOT_PRESENT:
			return "The resource required for this operation does not exist.\0";
		case ERROR_RESOURCE_ONLINE:
			return "The operation could not be completed because the cluster resource is online.\0";
		case ERROR_RESOURCE_PROPERTIES_STORED:
			return "The properties were stored but not all changes will take effect until the next time the resource is brought online.\0";
		case ERROR_RESOURCE_TYPE_NOT_FOUND:
			return "The specified resource type cannot be found in the image file.\0";
		case ERROR_RETRY:
			return "The operation could not be completed. A retry should be performed.\0";
		case ERROR_REVISION_MISMATCH:
			return "Indicates two revision levels are incompatible.\0";
		case ERROR_RING2_STACK_IN_USE:
			return "The ring 2 stack is in use.\0";
		case ERROR_RING2SEG_MUST_BE_MOVABLE:
			return "The code segment cannot be greater than or equal to 64K.\0";
		case ERROR_RMODE_APP:
			return "The specified program was written for an earlier version of Windows.\0";
		case ERROR_RPL_NOT_ALLOWED:
			return "Replication with a nonconfigured partner is not allowed.\0";
		case ERROR_RXACT_COMMIT_FAILURE:
			return "An internal security database corruption has been encountered.\0";
		case ERROR_RXACT_INVALID_STATE:
			return "The transaction state of a registry subtree is incompatible with the requested operation.\0";
		case ERROR_SAME_DRIVE:
			return "The system cannot join or substitute a drive to or for a directory on the same drive.\0";
		case ERROR_SCREEN_ALREADY_LOCKED:
			return "Screen already locked.\0";
		case ERROR_SECRET_TOO_LONG:
			return "The length of a secret exceeds the maximum length allowed.\0";
		case ERROR_SECTOR_NOT_FOUND:
			return "The drive cannot find the sector requested.\0";
		case ERROR_SEEK:
			return "The drive cannot locate a specific area or track on the disk.\0";
		case ERROR_SEEK_ON_DEVICE:
			return "The file pointer cannot be set on the specified device or file.\0";
		case ERROR_SEM_IS_SET:
			return "The semaphore is set and cannot be closed.\0";
		case ERROR_SEM_NOT_FOUND:
			return "The specified system semaphore name was not found.\0";
		case ERROR_SEM_OWNER_DIED:
			return "The previous ownership of this semaphore has ended.\0";
		case ERROR_SEM_TIMEOUT:
			return "The semaphore timeout period has expired.\0";
		case ERROR_SEM_USER_LIMIT:
			return "Insert the diskette for drive %1.\0";
		case ERROR_SERIAL_NO_DEVICE:
			return "No serial device was successfully initialized. The serial driver will unload.\0";
		case ERROR_SERVER_DISABLED:
			return "The server is currently disabled.\0";
		case ERROR_SERVER_HAS_OPEN_HANDLES:
			return "The server is in use and cannot be unloaded.\0";
		case ERROR_SERVER_NOT_DISABLED:
			return "The server is currently enabled.\0";
		case ERROR_SERVICE_ALREADY_RUNNING:
			return "An instance of the service is already running.\0";
		case ERROR_SERVICE_CANNOT_ACCEPT_CTRL:
			return "The service cannot accept control messages at this time.\0";
		case ERROR_SERVICE_DATABASE_LOCKED:
			return "The service database is locked.\0";
		case ERROR_SERVICE_DEPENDENCY_DELETED:
			return "The dependency service does not exist or has been marked for deletion.\0";
		case ERROR_SERVICE_DEPENDENCY_FAIL:
			return "The dependency service or group failed to start.\0";
		case ERROR_SERVICE_DISABLED:
			return "The service cannot be started, either because it is disabled or because it has no enabled devices associated with it.\0";
		case ERROR_SERVICE_DOES_NOT_EXIST:
			return "The specified service does not exist as an installed service.\0";
		case ERROR_SERVICE_EXISTS:
			return "The specified service already exists.\0";
		case ERROR_SERVICE_LOGON_FAILED:
			return "The service did not start due to a logon failure.\0";
		case ERROR_SERVICE_MARKED_FOR_DELETE:
			return "The specified service has been marked for deletion.\0";
		case ERROR_SERVICE_NEVER_STARTED:
			return "No attempts to start the service have been made since the last boot.\0";
		case ERROR_SERVICE_NO_THREAD:
			return "A thread could not be created for the service.\0";
		case ERROR_SERVICE_NOT_ACTIVE:
			return "The service has not been started.\0";
		case ERROR_SERVICE_NOT_FOUND:
			return "The specified service does not exist.\0";
		case ERROR_SERVICE_REQUEST_TIMEOUT:
			return "The service did not respond to the start or control request in a timely fashion.\0";
		case ERROR_SERVICE_SPECIFIC_ERROR:
			return "The service has returned a service-specific error code.\0";
		case ERROR_SERVICE_START_HANG:
			return "After starting, the service hung in a start-pending state.\0";
		case ERROR_SESSION_CREDENTIAL_CONFLICT:
			return "The credentials supplied conflict with an existing set of credentials.\0";
		case ERROR_SET_NOT_FOUND:
			return "The property set specified does not exist on the object.\0";
		case ERROR_SET_POWER_STATE_FAILED:
			return "The system BIOS failed an attempt to change the system power state.\0";
		case ERROR_SET_POWER_STATE_VETOED:
			return "An attempt to change the system power state was vetoed by another application or driver.\0";
		case ERROR_SETCOUNT_ON_BAD_LB:
			return "LB_SETCOUNT sent to non-lazy list box.\0";
		case ERROR_SETMARK_DETECTED:
			return "A tape access reached the end of a set of files.\0";
		case ERROR_SHARING_BUFFER_EXCEEDED:
			return "Too many files opened for sharing.\0";
		case ERROR_SHARING_PAUSED:
			return "The remote server has been paused or is in the process of being started.\0";
		case ERROR_SHARING_VIOLATION:
			return "The process cannot access the file because it is being used by another process.\0";
		case ERROR_SHUTDOWN_CLUSTER:
			return "The cluster is being shut down.\0";
		case ERROR_SHUTDOWN_IN_PROGRESS:
			return "A system shutdown is in progress.\0";
		case ERROR_SIGNAL_PENDING:
			return "A signal is already pending.\0";
		case ERROR_SIGNAL_REFUSED:
			return "The recipient process has refused the signal.\0";
		case ERROR_SINGLE_INSTANCE_APP:
			return "Cannot start more than one instance of the specified program.\0";
		case ERROR_SOME_NOT_MAPPED:
			return "Some mapping between account names and security IDs was not done.\0";
		case ERROR_SOURCE_ELEMENT_EMPTY:
			return "The indicated source element has no media.\0";
		case ERROR_SPECIAL_ACCOUNT:
			return "Cannot perform this operation on built-in accounts.\0";
		case ERROR_SPECIAL_GROUP:
			return "Cannot perform this operation on this built-in special group.\0";
		case ERROR_SPECIAL_USER:
			return "Cannot perform this operation n this built-in special user.\0";
		case ERROR_SPL_NO_ADDJOB:
			return "An AddJob call was not issued.\0";
		case ERROR_SPL_NO_STARTDOC:
			return "A StartDocPrinter call was not issued.\0";
		case ERROR_SPOOL_FILE_NOT_FOUND:
			return "The spool file was not found. at one time.\0";
		case ERROR_TOO_MANY_MODULES:
			return "Too many dynamic-link modules are attached to this program or dynamic-link module.\0";
		case ERROR_TOO_MANY_MUXWAITERS:
			return "DosMuxSemWait did not execute; too many semaphores are already set.\0";
		case ERROR_TOO_MANY_NAMES:
			return "The name limit for the local computer network adapter card was exceeded.\0";
		case ERROR_TOO_MANY_OPEN_FILES:
			return "The system cannot open the file.\0";
		case ERROR_TOO_MANY_POSTS:
			return "Too many posts were made to a semaphore.\0";
		case ERROR_TOO_MANY_SECRETS:
			return "The maximum number of secrets that may be stored in a single system.\0";
		case ERROR_STACK_OVERFLOW:
			return "Recursion too deep; the stack overflowed.\0";
		case ERROR_STATIC_INIT:
			return "The importation from the file failed.\0";
		case ERROR_SUBST_TO_JOIN:
			return "The system tried to SUBST a drive to a directory on a joined drive.\0";
		case ERROR_SUBST_TO_SUBST:
			return "The system tried to substitute a drive to a directory on a substituted drive.\0";
		case ERROR_SUCCESS:
			return "The operation completed successfully.\0";
		case ERROR_SUCCESS_REBOOT_REQUIRED:
			return "The requested operation is successful. Changes will not be effective until the system is rebooted.\0";
		case ERROR_SUCCESS_RESTART_REQUIRED:
			return "The requested operation is successful. Changes will not be effective until the service is restarted.\0";
		case ERROR_SWAPERROR:
			return "Error performing inpage operation.\0";
		case ERROR_SYSTEM_TRACE:
			return "System trace information was not specified in your CONFIG.SYS file, or tracing is disallowed.\0";
		case ERROR_TAG_NOT_FOUND:
			return "The specified tag was not found.\0";
		case ERROR_TAG_NOT_PRESENT:
			return "A required tag is not present.\0";
		case ERROR_THREAD_1_INACTIVE:
			return "The signal handler cannot be set.\0";
		case ERROR_TIMEOUT:
			return "This operation returned because the timeout period expired.\0";
		case ERROR_TLW_WITH_WSCHILD:
			return "Cannot create a top-level child window.\0";
		case ERROR_TOKEN_ALREADY_IN_USE:
			return "The token is already in use as a primary token.\0";
		case ERROR_TOO_MANY_CMDS:
			return "The network BIOS command limit has been reached.\0";
		case ERROR_TOO_MANY_CONTEXT_IDS:
			return "During a logon attempt, the user's security context accumulated too many security IDs.\0";
		case ERROR_TOO_MANY_LINKS:
			return "An attempt was made to create more links on a file than the file system supports.\0";
		case ERROR_TOO_MANY_LUIDS_REQUESTED:
			return "Too many local user identifiers (LUIDs) were requestedhas been exceeded.\0";
		case ERROR_TOO_MANY_SEM_REQUESTS:
			return "The semaphore cannot be set again.\0";
		case ERROR_TOO_MANY_SEMAPHORES:
			return "Cannot create another system semaphore.\0";
		case ERROR_TOO_MANY_SESS:
			return "The network BIOS session limit was exceeded.\0";
		case ERROR_TOO_MANY_SIDS:
			return "Too many security IDs have been specified.\0";
		case ERROR_TOO_MANY_TCBS:
			return "Cannot create another thread.\0";
		case ERROR_TRANSFORM_NOT_SUPPORTED:
			return "The requested transformation operation is not supported.\0";
		case ERROR_TRUST_FAILURE:
			return "The network logon failed.\0";
		case ERROR_TRUSTED_DOMAIN_FAILURE:
			return "The trust relationship between the primary domain and the trusted domain failed.\0";
		case ERROR_TRUSTED_RELATIONSHIP_FAILURE:
			return "The trust relationship between this workstation and the primary domain failed.\0";
		case ERROR_UNABLE_TO_CLEAN:
			return "The drive cannot be cleaned or does not support cleaning.\0";
		case ERROR_UNABLE_TO_LOCK_MEDIA:
			return "Unable to lock the media eject mechanism.\0";
		case ERROR_UNABLE_TO_UNLOAD_MEDIA:
			return "Unable to unload the media.\0";
		case ERROR_UNEXP_NET_ERR:
			return "An unexpected network error occurred.\0";
		case ERROR_UNKNOWN_COMPONENT:
			return "Component ID not registered.\0";
		case ERROR_UNKNOWN_FEATURE:
			return "Feature ID not registered.\0";
		case ERROR_UNKNOWN_PORT:
			return "The specified port is unknown.\0";
		case ERROR_UNKNOWN_PRINT_MONITOR:
			return "The specified print monitor is unknown.\0";
		case ERROR_UNKNOWN_PRINTER_DRIVER:
			return "The printer driver is unknown.\0";
		case ERROR_UNKNOWN_PRINTPROCESSOR:
			return "The print processor is unknown.\0";
		case ERROR_UNKNOWN_PRODUCT:
			return "Product code not registered.\0";
		case ERROR_UNKNOWN_PROPERTY:
			return "Unknown property.\0";
		case ERROR_UNKNOWN_REVISION:
			return "The revision level is unknown.\0";
		case ERROR_UNRECOGNIZED_MEDIA:
			return "The disk media is not recognized. It may not be formatted.\0";
		case ERROR_UNRECOGNIZED_VOLUME:
			return "The volume does not contain a recognized file system. Please make sure that all required file system drivers are loaded and that the volume is not corrupted.\0";
		case ERROR_USER_EXISTS:
			return "The specified user already exists.\0";
		case ERROR_USER_MAPPED_FILE:
			return "The requested operation cannot be performed on a file with a user-mapped section open.\0";
		case ERROR_VC_DISCONNECTED:
			return "The session was canceled.\0";
		case ERROR_WAIT_NO_CHILDREN:
			return "There are no child processes to wait for.\0";
		case ERROR_WINDOW_NOT_COMBOBOX:
			return "The window is not a combo box.\0";
		case ERROR_WINDOW_NOT_DIALOG:
			return "The window is not a valid dialog window.\0";
		case ERROR_WINDOW_OF_OTHER_THREAD:
			return "Invalid window; it belongs to other thread.\0";
		case ERROR_WINS_INTERNAL:
			return "WINS encountered an error while processing the command.\0";
		case ERROR_WMI_ALREADY_ENABLED:
			return "The WMI data block or event notification has already been enabled.\0";
		case ERROR_WMI_DP_FAILED:
			return "The WMI data provider failed to carry out the request.\0";
		case ERROR_WMI_DP_NOT_FOUND:
			return "The WMI data provider could not be located.\0";
		case ERROR_WMI_GUID_DISCONNECTED:
			return "The WMI data block is no longer available.\0";
		case ERROR_WMI_GUID_NOT_FOUND:
			return "The GUID passed was not recognized as valid by a WMI data provider.\0";
		case ERROR_WMI_INSTANCE_NOT_FOUND:
			return "The instance name passed was not recognized as valid by a WMI data provider.\0";
		case ERROR_WMI_INVALID_MOF:
			return "The WMI MOF information is not valid.\0";
		case ERROR_WMI_INVALID_REGINFO:
			return "The WMI registration information is not valid.\0";
		case ERROR_WMI_ITEMID_NOT_FOUND:
			return "The data item ID passed was not recognized as valid by a WMI data provider.\0";
		case ERROR_WMI_SERVER_UNAVAILABLE:
			return "The WMI data service is not available.\0";
		case ERROR_WMI_TRY_AGAIN:
			return "The WMI request could not be completed and should be retried.\0";
		case ERROR_WMI_UNRESOLVED_INSTANCE_REF:
			return "The WMI data provider references an instance set that has not been registered.\0";
		case ERROR_WORKING_SET_QUOTA:
			return "Insufficient quota to complete the requested service.\0";
		case ERROR_WRITE_FAULT:
			return "The system cannot write to the specified device.\0";
		case ERROR_WRITE_PROTECT:
			return "The media is write protected.\0";
		case ERROR_WRONG_DISK:
			return "The wrong diskette is in the drive. Insert %2 (Volume Serial Number: %3) into drive %1.\0";
		case ERROR_WRONG_EFS:
			return "The file was encrypted with a different encryption driver than is currently loaded.\0";
		case ERROR_WRONG_PASSWORD:
			return "Unable to update the password. The value provided as the current password is incorrect.\0";
		case OR_INVALID_OID:
			return "The object specified was not found.\0";
		case OR_INVALID_OXID:
			return "The object exporter specified was not found.\0";
		case OR_INVALID_SET:
			return "The object resolver set specified was not found.\0";
		case RPC_S_ADDRESS_ERROR:
			return "An addressing error occurred in the RPC server.\0";
		case RPC_S_ALREADY_LISTENING:
			return "The RPC server is already listening.\0";
		case RPC_S_ALREADY_REGISTERED:
			return "The object universal unique identifier (UUID) has already been registered.\0";
		case RPC_S_BINDING_HAS_NO_AUTH:
			return "The binding does not contain any authentication information.\0";
		case RPC_S_BINDING_INCOMPLETE:
			return "The binding handle does not contain all required information.\0";
		case RPC_S_CALL_CANCELLED:
			return "The remote procedure call was cancelled.\0";
		case RPC_S_CALL_FAILED:
			return "The remote procedure call failed.\0";
		case RPC_S_CALL_FAILED_DNE:
			return "The remote procedure call failed and did not execute.\0";
		case RPC_S_CALL_IN_PROGRESS:
			return "A remote procedure call is already in progress for this thread.\0";
		case RPC_S_CANNOT_SUPPORT:
			return "The requested operation is not supported.\0";
		case RPC_S_CANT_CREATE_ENDPOINT:
			return "The endpoint cannot be created.\0";
		case RPC_S_COMM_FAILURE:
			return "A communications failure occurred during a remote procedure call.\0";
		case RPC_S_DUPLICATE_ENDPOINT:
			return "The endpoint is a duplicate.\0";
		case RPC_S_ENTRY_ALREADY_EXISTS:
			return "The entry already exists.\0";
		case RPC_S_ENTRY_NOT_FOUND:
			return "The entry is not found.\0";
		case RPC_S_FP_DIV_ZERO:
			return "A floating-point operation at the RPC server caused a division by zero.\0";
		case RPC_S_FP_OVERFLOW:
			return "A floating-point overflow occurred at the RPC server.\0";
		case RPC_S_FP_UNDERFLOW:
			return "A floating-point underflow occurred at the RPC server.\0";
		case RPC_S_GROUP_MEMBER_NOT_FOUND:
			return "The group member was not found.\0";
		case RPC_S_INCOMPLETE_NAME:
			return "The entry name is incomplete.\0";
		case RPC_S_INTERFACE_NOT_FOUND:
			return "The interface was not found.\0";
		case RPC_S_INTERNAL_ERROR:
			return "An internal error occurred in a remote procedure call (RPC).\0";
		case RPC_S_INVALID_ASYNC_CALL:
			return "Invalid asynchronous RPC call handle for this operation.\0";
		case RPC_S_INVALID_ASYNC_HANDLE:
			return "Invalid asynchronous remote procedure call handle.\0";
		case RPC_S_INVALID_AUTH_IDENTITY: 
			return "The security context is invalid.\0";
		case RPC_S_INVALID_BINDING:
			return "The binding handle is invalid.\0";
		case RPC_S_INVALID_BOUND:
			return "The array bounds are invalid.\0";
		case RPC_S_INVALID_ENDPOINT_FORMAT:
			return "The endpoint format is invalid.\0";
		case RPC_S_INVALID_NAF_ID:
			return "The network address family is invalid.\0";
		case RPC_S_INVALID_NAME_SYNTAX:
			return "The name syntax is invalid.\0";
		case RPC_S_INVALID_NET_ADDR:
			return "The network address is invalid.\0";
		case RPC_S_INVALID_NETWORK_OPTIONS:
			return "The network options are invalid.\0";
		case RPC_S_INVALID_OBJECT:
			return "The object universal unique identifier (UUID) is the nil UUID.\0";
		case RPC_S_INVALID_RPC_PROTSEQ:
			return "The RPC protocol sequence is invalid.\0";
		case RPC_S_INVALID_STRING_BINDING:
			return "The string binding is invalid.\0";
		case RPC_S_INVALID_STRING_UUID:
			return "The string universal unique identifier (UUID) is invalid.\0";
		case RPC_S_INVALID_TAG:
			return "The tag is invalid.\0";
		case RPC_S_INVALID_TIMEOUT:
			return "The timeout value is invalid.\0";
		case RPC_S_INVALID_VERS_OPTION:
			return "The version option is invalid.\0";
		case RPC_S_MAX_CALLS_TOO_SMALL:
			return "The maximum number of calls is too small.\0";
		case RPC_S_NAME_SERVICE_UNAVAILABLE:
			return "The name service is unavailable.\0";
		case RPC_S_NO_BINDINGS:
			return "There are no bindings.\0";
		case RPC_S_NO_CALL_ACTIVE:
			return "There are no remote procedure calls active on this thread.\0";
		case RPC_S_NO_CONTEXT_AVAILABLE:
			return "No security context is available to allow impersonation.\0";
		case RPC_S_NO_ENDPOINT_FOUND:
			return "No endpoint was found.\0";
		case RPC_S_NO_ENTRY_NAME:
			return "The binding does not contain an entry name.\0";
		case RPC_S_NO_INTERFACES:
			return "No interfaces have been registered.\0";
		case RPC_S_NO_MORE_BINDINGS:
			return "There are no more bindings.\0";
		case RPC_S_NO_MORE_MEMBERS:
			return "There are no more members.\0";
		case RPC_S_NO_PRINC_NAME:
			return "No principal name registered.\0";
		case RPC_S_NO_PROTSEQS:
			return "There are no protocol sequences.\0";
		case RPC_S_NO_PROTSEQS_REGISTERED:
			return "No protocol sequences have been registered.\0";
		case RPC_S_NOT_ALL_OBJS_UNEXPORTED:
			return "There is nothing to unexport.\0";
		case RPC_S_NOT_CANCELLED:
			return "Thread is not canceled.\0";
		case RPC_S_NOT_LISTENING:
			return "The RPC server is not listening.\0";
		case RPC_S_NOT_RPC_ERROR:
			return "The error specified is not a valid Windows RPC error code.\0";
		case RPC_S_NOTHING_TO_EXPORT:
			return "No interfaces have been exported.\0";
		case RPC_S_OBJECT_NOT_FOUND:
			return "The object universal unique identifier (UUID) was not found.\0";
		case RPC_S_OUT_OF_RESOURCES:
			return "Not enough resources are available to complete this operation.\0";
		case RPC_S_PROCNUM_OUT_OF_RANGE:
			return "The procedure number is out of range.\0";
		case RPC_S_PROTOCOL_ERROR:
			return "A remote procedure call (RPC) protocol error occurred.\0";
		case RPC_S_PROTSEQ_NOT_FOUND:
			return "The RPC protocol sequence was not found.\0";
		case RPC_S_PROTSEQ_NOT_SUPPORTED:
			return "The RPC protocol sequence is not supported.\0";
		case RPC_S_SEC_PKG_ERROR:
			return "A security package specific error occurred.\0";
		case RPC_S_SEND_INCOMPLETE:
			return "Some data remains to be sent in the request buffer.\0";
		case RPC_S_SERVER_TOO_BUSY:
			return "The RPC server is too busy to complete this operation.\0";
		case RPC_S_SERVER_UNAVAILABLE:
			return "The RPC server is unavailable.\0";
		case RPC_S_STRING_TOO_LONG:
			return "The string is too long.\0";
		case RPC_S_TYPE_ALREADY_REGISTERED:
			return "The type universal unique identifier (UUID) has already been registered.\0";
		case RPC_S_UNKNOWN_AUTHN_LEVEL:
			return "The authentication level is unknown.\0";
		case RPC_S_UNKNOWN_AUTHN_SERVICE:
			return "The authentication service is unknown.\0";
		case RPC_S_UNKNOWN_AUTHN_TYPE:
			return "The authentication type is unknown.\0";
		case RPC_S_UNKNOWN_AUTHZ_SERVICE:
			return "The authorization service is unknown.\0";
		case RPC_S_UNKNOWN_IF:
			return "The interface is unknown.\0";
		case RPC_S_UNKNOWN_MGR_TYPE:
			return "The manager type is unknown.\0";
		case RPC_S_UNSUPPORTED_AUTHN_LEVEL:
			return "The requested authentication level is not supported.\0";
		case RPC_S_UNSUPPORTED_NAME_SYNTAX:
			return "The name syntax is not supported.\0";
		case RPC_S_UNSUPPORTED_TRANS_SYN:
			return "The transfer syntax is not supported by the RPC server.\0";
		case RPC_S_UNSUPPORTED_TYPE:
			return "The universal unique identifier (UUID) type is not supported.\0";
		case RPC_S_UUID_LOCAL_ONLY:
			return "A UUID that is valid only on this computer has been allocated.\0";
		case RPC_S_UUID_NO_ADDRESS:
			return "No network address is available to use to construct a universal unique identifier (UUID).\0";
		case RPC_S_WRONG_KIND_OF_BINDING:
			return "The binding handle is not the correct type.\0";
		case RPC_S_ZERO_DIVIDE:
			return "The RPC server attempted an integer division by zero.\0";
		case RPC_X_BAD_STUB_DATA:
			return "The stub received bad data.\0";
		case RPC_X_BYTE_COUNT_TOO_SMALL:
			return "The byte count is too small.\0";
		case RPC_X_ENUM_VALUE_OUT_OF_RANGE:
			return "The enumeration value is out of range.\0";
		case RPC_X_INVALID_ES_ACTION:
			return "Invalid operation on the encoding/decoding handle.\0";
		case RPC_X_INVALID_PIPE_OBJECT:
			return "The RPC pipe object is invalid or corrupted.\0";
		case RPC_X_NO_MORE_ENTRIES:
			return "The list of RPC servers available for the binding of auto handles has been exhausted.\0";
		case RPC_X_NULL_REF_POINTER:
			return "A null reference pointer was passed to the stub.\0";
		case RPC_X_PIPE_CLOSED:
			return "The RPC pipe object has already been closed.\0";
		case RPC_X_PIPE_DISCIPLINE_ERROR:
			return "The RPC call completed before all pipes were processed.\0";
		case RPC_X_PIPE_EMPTY:
			return "No more data is available from the RPC pipe.\0";
		case RPC_X_SS_CANNOT_GET_CALL_HANDLE:
			return "The stub is unable to get the remote procedure call handle.\0";
		case RPC_X_SS_CHAR_TRANS_OPEN_FAIL:
			return "Unable to open the character translation table file.\0";
		case RPC_X_SS_CHAR_TRANS_SHORT_FILE:
			return "The file containing the character translation table has fewer than bytes.\0";
		case RPC_X_SS_CONTEXT_DAMAGED:
			return "The context handle changed during a remote procedure call.\0";
		case RPC_X_SS_HANDLES_MISMATCH:
			return "The binding handles passed to a remote procedure call do not match.\0";
		case RPC_X_SS_IN_NULL_CONTEXT:
			return "A null context handle was passed from the client to the host during a remote procedure call.\0";
		case RPC_X_WRONG_ES_VERSION:
			return "Incompatible version of the serializing package.\0";
		case RPC_X_WRONG_PIPE_ORDER:
			return "An invalid operation was attempted on an RPC pipe object.\0";
		case RPC_X_WRONG_PIPE_VERSION:
			return "Unsupported RPC pipe version.\0";
		case RPC_X_WRONG_STUB_VERSION:
			return "Incompatible version of the RPC stub.\0";
		default:
			return "Unrecognized error value.\0";
	}
}