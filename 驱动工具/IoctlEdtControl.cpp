#include "stdafx.h"
#include "IoctlEdtControl.h"
#include <wx/textctrl.h>
#include <wx/string.h>
#include <map>
#include <bitset>
#include <WinIoCtl.h>
#include "KbdIoctlCode.h"
#include "PrintDevice.h"
#include "HidDevice.h"
#include "ddpar.h"

CIoctlEdtControl::CIoctlEdtControl(wxTextCtrl * code, wxTextCtrl * mnemonic, wxTextCtrl * devType, wxTextCtrl * function, wxTextCtrl * method, wxTextCtrl * access, wxTextCtrl * layout):
	m_pEdtCode(code), m_pEdtMnemonic(mnemonic), m_pEdtDevType(devType), m_pEdtFunction(function), m_pEdtMethod(method), m_pEdtAccess(access), m_pEdtLayout(layout)
{
	InitMapData();
}

CIoctlEdtControl::~CIoctlEdtControl()
{
}

void CIoctlEdtControl::InitMapData()
{
#define INSERT(mapdata, first)\
	mapdata.insert(std::pair<ULONG, wxString>(first, wxT(#first)))

	//Access
	INSERT(m_mapAccess, FILE_ANY_ACCESS);
	INSERT(m_mapAccess, FILE_READ_ACCESS);
	INSERT(m_mapAccess, FILE_WRITE_ACCESS);
	INSERT(m_mapAccess, FILE_READ_ACCESS+FILE_WRITE_ACCESS);

	//Method
	INSERT(m_mapMethod, METHOD_BUFFERED);
	INSERT(m_mapMethod, METHOD_IN_DIRECT);
	INSERT(m_mapMethod, METHOD_OUT_DIRECT);
	INSERT(m_mapMethod, METHOD_NEITHER);

	//DeviceType
	{
		INSERT(m_mapDeviceType, FILE_DEVICE_BEEP);
		INSERT(m_mapDeviceType, FILE_DEVICE_CD_ROM);
		INSERT(m_mapDeviceType, FILE_DEVICE_CD_ROM_FILE_SYSTEM);
		INSERT(m_mapDeviceType, FILE_DEVICE_CONTROLLER);
		INSERT(m_mapDeviceType, FILE_DEVICE_DATALINK);
		INSERT(m_mapDeviceType, FILE_DEVICE_DFS);
		INSERT(m_mapDeviceType, FILE_DEVICE_DISK);
		INSERT(m_mapDeviceType, FILE_DEVICE_DISK_FILE_SYSTEM);
		INSERT(m_mapDeviceType, FILE_DEVICE_FILE_SYSTEM);
		INSERT(m_mapDeviceType, FILE_DEVICE_INPORT_PORT);
		INSERT(m_mapDeviceType, FILE_DEVICE_KEYBOARD);
		INSERT(m_mapDeviceType, FILE_DEVICE_MAILSLOT);
		INSERT(m_mapDeviceType, FILE_DEVICE_MIDI_IN);
		INSERT(m_mapDeviceType, FILE_DEVICE_MIDI_OUT);
		INSERT(m_mapDeviceType, FILE_DEVICE_MOUSE);
		INSERT(m_mapDeviceType, FILE_DEVICE_MULTI_UNC_PROVIDER);
		INSERT(m_mapDeviceType, FILE_DEVICE_NAMED_PIPE);
		INSERT(m_mapDeviceType, FILE_DEVICE_NETWORK);
		INSERT(m_mapDeviceType, FILE_DEVICE_NETWORK_BROWSER);
		INSERT(m_mapDeviceType, FILE_DEVICE_NETWORK_FILE_SYSTEM);
		INSERT(m_mapDeviceType, FILE_DEVICE_NULL);
		INSERT(m_mapDeviceType, FILE_DEVICE_PARALLEL_PORT);
		INSERT(m_mapDeviceType, FILE_DEVICE_PHYSICAL_NETCARD);
		INSERT(m_mapDeviceType, FILE_DEVICE_PRINTER);
		INSERT(m_mapDeviceType, FILE_DEVICE_SCANNER);
		INSERT(m_mapDeviceType, FILE_DEVICE_SERIAL_MOUSE_PORT);
		INSERT(m_mapDeviceType, FILE_DEVICE_SERIAL_PORT);
		INSERT(m_mapDeviceType, FILE_DEVICE_SCREEN);
		INSERT(m_mapDeviceType, FILE_DEVICE_SOUND);
		INSERT(m_mapDeviceType, FILE_DEVICE_STREAMS);
		INSERT(m_mapDeviceType, FILE_DEVICE_TAPE);
		INSERT(m_mapDeviceType, FILE_DEVICE_TAPE_FILE_SYSTEM);
		INSERT(m_mapDeviceType, FILE_DEVICE_TRANSPORT);
		INSERT(m_mapDeviceType, FILE_DEVICE_UNKNOWN);
		INSERT(m_mapDeviceType, FILE_DEVICE_VIDEO);
		INSERT(m_mapDeviceType, FILE_DEVICE_VIRTUAL_DISK);
		INSERT(m_mapDeviceType, FILE_DEVICE_WAVE_IN);
		INSERT(m_mapDeviceType, FILE_DEVICE_WAVE_OUT);
		INSERT(m_mapDeviceType, FILE_DEVICE_8042_PORT);
		INSERT(m_mapDeviceType, FILE_DEVICE_NETWORK_REDIRECTOR);
		INSERT(m_mapDeviceType, FILE_DEVICE_BATTERY);
		INSERT(m_mapDeviceType, FILE_DEVICE_BUS_EXTENDER);
		INSERT(m_mapDeviceType, FILE_DEVICE_MODEM);
		INSERT(m_mapDeviceType, FILE_DEVICE_VDM);
		INSERT(m_mapDeviceType, FILE_DEVICE_MASS_STORAGE);
		INSERT(m_mapDeviceType, FILE_DEVICE_SMB);
		INSERT(m_mapDeviceType, FILE_DEVICE_KS);
		INSERT(m_mapDeviceType, FILE_DEVICE_CHANGER);
		INSERT(m_mapDeviceType, FILE_DEVICE_SMARTCARD);
		INSERT(m_mapDeviceType, FILE_DEVICE_ACPI);
		INSERT(m_mapDeviceType, FILE_DEVICE_DVD);
		INSERT(m_mapDeviceType, FILE_DEVICE_FULLSCREEN_VIDEO);
		INSERT(m_mapDeviceType, FILE_DEVICE_DFS_FILE_SYSTEM);
		INSERT(m_mapDeviceType, FILE_DEVICE_DFS_VOLUME);
		INSERT(m_mapDeviceType, FILE_DEVICE_SERENUM);
		INSERT(m_mapDeviceType, FILE_DEVICE_TERMSRV);
		INSERT(m_mapDeviceType, FILE_DEVICE_KSEC);
		INSERT(m_mapDeviceType, FILE_DEVICE_FIPS);
		INSERT(m_mapDeviceType, FILE_DEVICE_INFINIBAND);
		INSERT(m_mapDeviceType, FILE_DEVICE_VMBUS);
		INSERT(m_mapDeviceType, FILE_DEVICE_CRYPT_PROVIDER);
		INSERT(m_mapDeviceType, FILE_DEVICE_WPD);
		INSERT(m_mapDeviceType, FILE_DEVICE_BLUETOOTH);
		INSERT(m_mapDeviceType, FILE_DEVICE_MT_COMPOSITE);
		INSERT(m_mapDeviceType, FILE_DEVICE_MT_TRANSPORT);
		INSERT(m_mapDeviceType, FILE_DEVICE_BIOMETRIC);
		INSERT(m_mapDeviceType, FILE_DEVICE_PMI);
		INSERT(m_mapDeviceType, FILE_DEVICE_DOT4);
	}
#undef INSERT

#define INSERT(mapdata, mapdata2, first)\
	mapdata.insert(std::pair<ULONG, wxString>(first, wxT(#first)));\
	mapdata2.insert(std::pair<wxString, ULONG>(wxT(#first), first));

	//Code To Mnemonic
	{
		//Kbd
		{
			INSERT(m_mapCodeAndMnemonic, m_mapMnemonicToCode, IOCTL_KEYBOARD_QUERY_ATTRIBUTES);
			INSERT(m_mapCodeAndMnemonic, m_mapMnemonicToCode, IOCTL_KEYBOARD_SET_TYPEMATIC);
			INSERT(m_mapCodeAndMnemonic, m_mapMnemonicToCode, IOCTL_KEYBOARD_SET_INDICATORS);
			INSERT(m_mapCodeAndMnemonic, m_mapMnemonicToCode, IOCTL_KEYBOARD_QUERY_TYPEMATIC);
			INSERT(m_mapCodeAndMnemonic, m_mapMnemonicToCode, IOCTL_KEYBOARD_QUERY_INDICATORS);
			INSERT(m_mapCodeAndMnemonic, m_mapMnemonicToCode, IOCTL_KEYBOARD_QUERY_INDICATOR_TRANSLATION);
			INSERT(m_mapCodeAndMnemonic, m_mapMnemonicToCode, IOCTL_KEYBOARD_INSERT_DATA);
			INSERT(m_mapCodeAndMnemonic, m_mapMnemonicToCode, IOCTL_KEYBOARD_QUERY_EXTENDED_ATTRIBUTES);
			INSERT(m_mapCodeAndMnemonic, m_mapMnemonicToCode, IOCTL_KEYBOARD_QUERY_IME_STATUS);
			INSERT(m_mapCodeAndMnemonic, m_mapMnemonicToCode, IOCTL_KEYBOARD_SET_IME_STATUS);
			INSERT(m_mapCodeAndMnemonic, m_mapMnemonicToCode, IOCTL_KEYBOARD_QUERY_ATTRIBUTES);
			INSERT(m_mapCodeAndMnemonic, m_mapMnemonicToCode, IOCTL_KEYBOARD_SET_TYPEMATIC);
			INSERT(m_mapCodeAndMnemonic, m_mapMnemonicToCode, IOCTL_KEYBOARD_SET_INDICATORS);
			INSERT(m_mapCodeAndMnemonic, m_mapMnemonicToCode, IOCTL_KEYBOARD_QUERY_TYPEMATIC);
			INSERT(m_mapCodeAndMnemonic, m_mapMnemonicToCode, IOCTL_KEYBOARD_QUERY_INDICATORS);
			INSERT(m_mapCodeAndMnemonic, m_mapMnemonicToCode, IOCTL_KEYBOARD_QUERY_INDICATOR_TRANSLATION);
			INSERT(m_mapCodeAndMnemonic, m_mapMnemonicToCode, IOCTL_KEYBOARD_INSERT_DATA);
			INSERT(m_mapCodeAndMnemonic, m_mapMnemonicToCode, IOCTL_KEYBOARD_QUERY_EXTENDED_ATTRIBUTES);
			INSERT(m_mapCodeAndMnemonic, m_mapMnemonicToCode, IOCTL_KEYBOARD_QUERY_IME_STATUS);
			INSERT(m_mapCodeAndMnemonic, m_mapMnemonicToCode, IOCTL_KEYBOARD_SET_IME_STATUS);
		}

		//PrintDevice
		{
			INSERT(m_mapCodeAndMnemonic, m_mapMnemonicToCode, IOCTL_DOT4_CREATE_SOCKET);
			INSERT(m_mapCodeAndMnemonic, m_mapMnemonicToCode, IOCTL_DOT4_DESTROY_SOCKET);
			INSERT(m_mapCodeAndMnemonic, m_mapMnemonicToCode, IOCTL_DOT4_WAIT_FOR_CHANNEL);
			INSERT(m_mapCodeAndMnemonic, m_mapMnemonicToCode, IOCTL_DOT4_OPEN_CHANNEL);
			INSERT(m_mapCodeAndMnemonic, m_mapMnemonicToCode, IOCTL_DOT4_CLOSE_CHANNEL);
			INSERT(m_mapCodeAndMnemonic, m_mapMnemonicToCode, IOCTL_DOT4_READ);
			INSERT(m_mapCodeAndMnemonic, m_mapMnemonicToCode, IOCTL_DOT4_WRITE);
			INSERT(m_mapCodeAndMnemonic, m_mapMnemonicToCode, IOCTL_DOT4_ADD_ACTIVITY_BROADCAST);
			INSERT(m_mapCodeAndMnemonic, m_mapMnemonicToCode, IOCTL_DOT4_REMOVE_ACTIVITY_BROADCAST);
			INSERT(m_mapCodeAndMnemonic, m_mapMnemonicToCode, IOCTL_DOT4_WAIT_ACTIVITY_BROADCAST);
		}

		//HIDClass
		{
			INSERT(m_mapCodeAndMnemonic, m_mapMnemonicToCode, IOCTL_HID_GET_DRIVER_CONFIG);
			INSERT(m_mapCodeAndMnemonic, m_mapMnemonicToCode, IOCTL_HID_SET_DRIVER_CONFIG);
			INSERT(m_mapCodeAndMnemonic, m_mapMnemonicToCode, IOCTL_HID_GET_POLL_FREQUENCY_MSEC);
			INSERT(m_mapCodeAndMnemonic, m_mapMnemonicToCode, IOCTL_HID_SET_POLL_FREQUENCY_MSEC);
			INSERT(m_mapCodeAndMnemonic, m_mapMnemonicToCode, IOCTL_GET_NUM_DEVICE_INPUT_BUFFERS);
			INSERT(m_mapCodeAndMnemonic, m_mapMnemonicToCode, IOCTL_SET_NUM_DEVICE_INPUT_BUFFERS);
			INSERT(m_mapCodeAndMnemonic, m_mapMnemonicToCode, IOCTL_HID_GET_COLLECTION_INFORMATION);
			INSERT(m_mapCodeAndMnemonic, m_mapMnemonicToCode, IOCTL_HID_ENABLE_WAKE_ON_SX);
			INSERT(m_mapCodeAndMnemonic, m_mapMnemonicToCode, IOCTL_HID_SET_S0_IDLE_TIMEOUT);
			INSERT(m_mapCodeAndMnemonic, m_mapMnemonicToCode, IOCTL_HID_GET_COLLECTION_DESCRIPTOR);
			INSERT(m_mapCodeAndMnemonic, m_mapMnemonicToCode, IOCTL_HID_FLUSH_QUEUE);
			INSERT(m_mapCodeAndMnemonic, m_mapMnemonicToCode, IOCTL_HID_SET_FEATURE);
			INSERT(m_mapCodeAndMnemonic, m_mapMnemonicToCode, IOCTL_HID_SET_OUTPUT_REPORT);
			INSERT(m_mapCodeAndMnemonic, m_mapMnemonicToCode, IOCTL_HID_GET_FEATURE);
			INSERT(m_mapCodeAndMnemonic, m_mapMnemonicToCode, IOCTL_GET_PHYSICAL_DESCRIPTOR);
			INSERT(m_mapCodeAndMnemonic, m_mapMnemonicToCode, IOCTL_HID_GET_HARDWARE_ID);
			INSERT(m_mapCodeAndMnemonic, m_mapMnemonicToCode, IOCTL_HID_GET_INPUT_REPORT);
			INSERT(m_mapCodeAndMnemonic, m_mapMnemonicToCode, IOCTL_HID_GET_OUTPUT_REPORT);
			INSERT(m_mapCodeAndMnemonic, m_mapMnemonicToCode, IOCTL_HID_GET_MANUFACTURER_STRING);
			INSERT(m_mapCodeAndMnemonic, m_mapMnemonicToCode, IOCTL_HID_GET_PRODUCT_STRING);
			INSERT(m_mapCodeAndMnemonic, m_mapMnemonicToCode, IOCTL_HID_GET_SERIALNUMBER_STRING);
			INSERT(m_mapCodeAndMnemonic, m_mapMnemonicToCode, IOCTL_HID_GET_INDEXED_STRING);
			INSERT(m_mapCodeAndMnemonic, m_mapMnemonicToCode, IOCTL_HID_GET_MS_GENRE_DESCRIPTOR);
			INSERT(m_mapCodeAndMnemonic, m_mapMnemonicToCode, IOCTL_HID_ENABLE_SECURE_READ);
			INSERT(m_mapCodeAndMnemonic, m_mapMnemonicToCode, IOCTL_HID_DISABLE_SECURE_READ);
			INSERT(m_mapCodeAndMnemonic, m_mapMnemonicToCode, IOCTL_HID_DEVICERESET_NOTIFICATION);
		}

		//ddpar
		{
			INSERT(m_mapCodeAndMnemonic, m_mapMnemonicToCode, IOCTL_PAR_QUERY_INFORMATION);
			INSERT(m_mapCodeAndMnemonic, m_mapMnemonicToCode, IOCTL_PAR_SET_INFORMATION);
			INSERT(m_mapCodeAndMnemonic, m_mapMnemonicToCode, IOCTL_PAR_QUERY_DEVICE_ID);
			INSERT(m_mapCodeAndMnemonic, m_mapMnemonicToCode, IOCTL_PAR_QUERY_DEVICE_ID_SIZE);
			INSERT(m_mapCodeAndMnemonic, m_mapMnemonicToCode, IOCTL_IEEE1284_GET_MODE);
			INSERT(m_mapCodeAndMnemonic, m_mapMnemonicToCode, IOCTL_IEEE1284_NEGOTIATE);
			INSERT(m_mapCodeAndMnemonic, m_mapMnemonicToCode, IOCTL_PAR_SET_WRITE_ADDRESS);
			INSERT(m_mapCodeAndMnemonic, m_mapMnemonicToCode, IOCTL_PAR_SET_READ_ADDRESS);
			INSERT(m_mapCodeAndMnemonic, m_mapMnemonicToCode, IOCTL_PAR_GET_DEVICE_CAPS);
			INSERT(m_mapCodeAndMnemonic, m_mapMnemonicToCode, IOCTL_PAR_GET_DEFAULT_MODES);
			INSERT(m_mapCodeAndMnemonic, m_mapMnemonicToCode, IOCTL_PAR_PING);
			INSERT(m_mapCodeAndMnemonic, m_mapMnemonicToCode, IOCTL_PAR_QUERY_RAW_DEVICE_ID);
			INSERT(m_mapCodeAndMnemonic, m_mapMnemonicToCode, IOCTL_PAR_ECP_HOST_RECOVERY);
			INSERT(m_mapCodeAndMnemonic, m_mapMnemonicToCode, IOCTL_PAR_GET_READ_ADDRESS);
			INSERT(m_mapCodeAndMnemonic, m_mapMnemonicToCode, IOCTL_PAR_GET_WRITE_ADDRESS);
			INSERT(m_mapCodeAndMnemonic, m_mapMnemonicToCode, IOCTL_PAR_TEST);
			INSERT(m_mapCodeAndMnemonic, m_mapMnemonicToCode, IOCTL_PAR_IS_PORT_FREE);
			INSERT(m_mapCodeAndMnemonic, m_mapMnemonicToCode, IOCTL_PAR_QUERY_LOCATION);
		}

		//basic
		{
			INSERT(m_mapCodeAndMnemonic, m_mapMnemonicToCode, IOCTL_STORAGE_CHECK_VERIFY);
			INSERT(m_mapCodeAndMnemonic, m_mapMnemonicToCode, IOCTL_STORAGE_CHECK_VERIFY2);
			INSERT(m_mapCodeAndMnemonic, m_mapMnemonicToCode, IOCTL_STORAGE_MEDIA_REMOVAL);
			INSERT(m_mapCodeAndMnemonic, m_mapMnemonicToCode, IOCTL_STORAGE_EJECT_MEDIA);
			INSERT(m_mapCodeAndMnemonic, m_mapMnemonicToCode, IOCTL_STORAGE_LOAD_MEDIA);
			INSERT(m_mapCodeAndMnemonic, m_mapMnemonicToCode, IOCTL_STORAGE_LOAD_MEDIA2);
			INSERT(m_mapCodeAndMnemonic, m_mapMnemonicToCode, IOCTL_STORAGE_RESERVE);
			INSERT(m_mapCodeAndMnemonic, m_mapMnemonicToCode, IOCTL_STORAGE_RELEASE);
			INSERT(m_mapCodeAndMnemonic, m_mapMnemonicToCode, IOCTL_STORAGE_FIND_NEW_DEVICES);
			INSERT(m_mapCodeAndMnemonic, m_mapMnemonicToCode, IOCTL_STORAGE_EJECTION_CONTROL);
			INSERT(m_mapCodeAndMnemonic, m_mapMnemonicToCode, IOCTL_STORAGE_MCN_CONTROL);
			INSERT(m_mapCodeAndMnemonic, m_mapMnemonicToCode, IOCTL_STORAGE_GET_MEDIA_TYPES);
			INSERT(m_mapCodeAndMnemonic, m_mapMnemonicToCode, IOCTL_STORAGE_GET_MEDIA_TYPES_EX);
			INSERT(m_mapCodeAndMnemonic, m_mapMnemonicToCode, IOCTL_STORAGE_GET_MEDIA_SERIAL_NUMBER);
			INSERT(m_mapCodeAndMnemonic, m_mapMnemonicToCode, IOCTL_STORAGE_GET_HOTPLUG_INFO);
			INSERT(m_mapCodeAndMnemonic, m_mapMnemonicToCode, IOCTL_STORAGE_SET_HOTPLUG_INFO);
			INSERT(m_mapCodeAndMnemonic, m_mapMnemonicToCode, IOCTL_STORAGE_RESET_BUS);
			INSERT(m_mapCodeAndMnemonic, m_mapMnemonicToCode, IOCTL_STORAGE_RESET_DEVICE);
			INSERT(m_mapCodeAndMnemonic, m_mapMnemonicToCode, IOCTL_STORAGE_BREAK_RESERVATION);
			INSERT(m_mapCodeAndMnemonic, m_mapMnemonicToCode, IOCTL_STORAGE_PERSISTENT_RESERVE_IN);
			INSERT(m_mapCodeAndMnemonic, m_mapMnemonicToCode, IOCTL_STORAGE_PERSISTENT_RESERVE_OUT);
			INSERT(m_mapCodeAndMnemonic, m_mapMnemonicToCode, IOCTL_STORAGE_GET_DEVICE_NUMBER);
			INSERT(m_mapCodeAndMnemonic, m_mapMnemonicToCode, IOCTL_STORAGE_PREDICT_FAILURE);
			INSERT(m_mapCodeAndMnemonic, m_mapMnemonicToCode, IOCTL_STORAGE_READ_CAPACITY);
			INSERT(m_mapCodeAndMnemonic, m_mapMnemonicToCode, IOCTL_STORAGE_QUERY_PROPERTY);
			INSERT(m_mapCodeAndMnemonic, m_mapMnemonicToCode, IOCTL_STORAGE_MANAGE_DATA_SET_ATTRIBUTES);
			INSERT(m_mapCodeAndMnemonic, m_mapMnemonicToCode, IOCTL_STORAGE_GET_BC_PROPERTIES);
			INSERT(m_mapCodeAndMnemonic, m_mapMnemonicToCode, IOCTL_STORAGE_ALLOCATE_BC_STREAM);
			INSERT(m_mapCodeAndMnemonic, m_mapMnemonicToCode, IOCTL_STORAGE_FREE_BC_STREAM);
			INSERT(m_mapCodeAndMnemonic, m_mapMnemonicToCode, IOCTL_STORAGE_CHECK_PRIORITY_HINT_SUPPORT);
			INSERT(m_mapCodeAndMnemonic, m_mapMnemonicToCode, OBSOLETE_IOCTL_STORAGE_RESET_BUS);
			INSERT(m_mapCodeAndMnemonic, m_mapMnemonicToCode, OBSOLETE_IOCTL_STORAGE_RESET_DEVICE);
			INSERT(m_mapCodeAndMnemonic, m_mapMnemonicToCode, IOCTL_DISK_GET_DRIVE_GEOMETRY);
			INSERT(m_mapCodeAndMnemonic, m_mapMnemonicToCode, IOCTL_DISK_GET_PARTITION_INFO);
			INSERT(m_mapCodeAndMnemonic, m_mapMnemonicToCode, IOCTL_DISK_SET_PARTITION_INFO);
			INSERT(m_mapCodeAndMnemonic, m_mapMnemonicToCode, IOCTL_DISK_GET_DRIVE_LAYOUT);
			INSERT(m_mapCodeAndMnemonic, m_mapMnemonicToCode, IOCTL_DISK_SET_DRIVE_LAYOUT);
			INSERT(m_mapCodeAndMnemonic, m_mapMnemonicToCode, IOCTL_DISK_VERIFY);
			INSERT(m_mapCodeAndMnemonic, m_mapMnemonicToCode, IOCTL_DISK_FORMAT_TRACKS);
			INSERT(m_mapCodeAndMnemonic, m_mapMnemonicToCode, IOCTL_DISK_REASSIGN_BLOCKS);
			INSERT(m_mapCodeAndMnemonic, m_mapMnemonicToCode, IOCTL_DISK_PERFORMANCE);
			INSERT(m_mapCodeAndMnemonic, m_mapMnemonicToCode, IOCTL_DISK_IS_WRITABLE);
			INSERT(m_mapCodeAndMnemonic, m_mapMnemonicToCode, IOCTL_DISK_LOGGING);
			INSERT(m_mapCodeAndMnemonic, m_mapMnemonicToCode, IOCTL_DISK_FORMAT_TRACKS_EX);
			INSERT(m_mapCodeAndMnemonic, m_mapMnemonicToCode, IOCTL_DISK_HISTOGRAM_STRUCTURE);
			INSERT(m_mapCodeAndMnemonic, m_mapMnemonicToCode, IOCTL_DISK_HISTOGRAM_DATA);
			INSERT(m_mapCodeAndMnemonic, m_mapMnemonicToCode, IOCTL_DISK_HISTOGRAM_RESET);
			INSERT(m_mapCodeAndMnemonic, m_mapMnemonicToCode, IOCTL_DISK_REQUEST_STRUCTURE);
			INSERT(m_mapCodeAndMnemonic, m_mapMnemonicToCode, IOCTL_DISK_REQUEST_DATA);
			INSERT(m_mapCodeAndMnemonic, m_mapMnemonicToCode, IOCTL_DISK_PERFORMANCE_OFF);
			INSERT(m_mapCodeAndMnemonic, m_mapMnemonicToCode, IOCTL_DISK_CONTROLLER_NUMBER);
			INSERT(m_mapCodeAndMnemonic, m_mapMnemonicToCode, SMART_GET_VERSION);
			INSERT(m_mapCodeAndMnemonic, m_mapMnemonicToCode, SMART_SEND_DRIVE_COMMAND);
			INSERT(m_mapCodeAndMnemonic, m_mapMnemonicToCode, SMART_RCV_DRIVE_DATA);
			INSERT(m_mapCodeAndMnemonic, m_mapMnemonicToCode, IOCTL_DISK_GET_PARTITION_INFO_EX);
			INSERT(m_mapCodeAndMnemonic, m_mapMnemonicToCode, IOCTL_DISK_SET_PARTITION_INFO_EX);
			INSERT(m_mapCodeAndMnemonic, m_mapMnemonicToCode, IOCTL_DISK_GET_DRIVE_LAYOUT_EX);
			INSERT(m_mapCodeAndMnemonic, m_mapMnemonicToCode, IOCTL_DISK_SET_DRIVE_LAYOUT_EX);
			INSERT(m_mapCodeAndMnemonic, m_mapMnemonicToCode, IOCTL_DISK_CREATE_DISK);
			INSERT(m_mapCodeAndMnemonic, m_mapMnemonicToCode, IOCTL_DISK_GET_LENGTH_INFO);
			INSERT(m_mapCodeAndMnemonic, m_mapMnemonicToCode, IOCTL_DISK_GET_DRIVE_GEOMETRY_EX);
			INSERT(m_mapCodeAndMnemonic, m_mapMnemonicToCode, IOCTL_DISK_REASSIGN_BLOCKS_EX);
			INSERT(m_mapCodeAndMnemonic, m_mapMnemonicToCode, IOCTL_DISK_UPDATE_DRIVE_SIZE);
			INSERT(m_mapCodeAndMnemonic, m_mapMnemonicToCode, IOCTL_DISK_GROW_PARTITION);
			INSERT(m_mapCodeAndMnemonic, m_mapMnemonicToCode, IOCTL_DISK_GET_CACHE_INFORMATION);
			INSERT(m_mapCodeAndMnemonic, m_mapMnemonicToCode, IOCTL_DISK_SET_CACHE_INFORMATION);
			INSERT(m_mapCodeAndMnemonic, m_mapMnemonicToCode, OBSOLETE_DISK_GET_WRITE_CACHE_STATE);
			INSERT(m_mapCodeAndMnemonic, m_mapMnemonicToCode, IOCTL_DISK_DELETE_DRIVE_LAYOUT);
			INSERT(m_mapCodeAndMnemonic, m_mapMnemonicToCode, IOCTL_DISK_UPDATE_PROPERTIES);
			INSERT(m_mapCodeAndMnemonic, m_mapMnemonicToCode, IOCTL_DISK_FORMAT_DRIVE);
			INSERT(m_mapCodeAndMnemonic, m_mapMnemonicToCode, IOCTL_DISK_SENSE_DEVICE);
			INSERT(m_mapCodeAndMnemonic, m_mapMnemonicToCode, IOCTL_DISK_CHECK_VERIFY);
			INSERT(m_mapCodeAndMnemonic, m_mapMnemonicToCode, IOCTL_DISK_MEDIA_REMOVAL);
			INSERT(m_mapCodeAndMnemonic, m_mapMnemonicToCode, IOCTL_DISK_EJECT_MEDIA);
			INSERT(m_mapCodeAndMnemonic, m_mapMnemonicToCode, IOCTL_DISK_LOAD_MEDIA);
			INSERT(m_mapCodeAndMnemonic, m_mapMnemonicToCode, IOCTL_DISK_RESERVE);
			INSERT(m_mapCodeAndMnemonic, m_mapMnemonicToCode, IOCTL_DISK_RELEASE);
			INSERT(m_mapCodeAndMnemonic, m_mapMnemonicToCode, IOCTL_DISK_FIND_NEW_DEVICES);
			INSERT(m_mapCodeAndMnemonic, m_mapMnemonicToCode, IOCTL_DISK_GET_MEDIA_TYPES);
			INSERT(m_mapCodeAndMnemonic, m_mapMnemonicToCode, IOCTL_DISK_RESET_SNAPSHOT_INFO);
			INSERT(m_mapCodeAndMnemonic, m_mapMnemonicToCode, IOCTL_CHANGER_GET_PARAMETERS);
			INSERT(m_mapCodeAndMnemonic, m_mapMnemonicToCode, IOCTL_CHANGER_GET_STATUS);
			INSERT(m_mapCodeAndMnemonic, m_mapMnemonicToCode, IOCTL_CHANGER_GET_PRODUCT_DATA);
			INSERT(m_mapCodeAndMnemonic, m_mapMnemonicToCode, IOCTL_CHANGER_SET_ACCESS);
			INSERT(m_mapCodeAndMnemonic, m_mapMnemonicToCode, IOCTL_CHANGER_GET_ELEMENT_STATUS);
			INSERT(m_mapCodeAndMnemonic, m_mapMnemonicToCode, IOCTL_CHANGER_INITIALIZE_ELEMENT_STATUS);
			INSERT(m_mapCodeAndMnemonic, m_mapMnemonicToCode, IOCTL_CHANGER_SET_POSITION);
			INSERT(m_mapCodeAndMnemonic, m_mapMnemonicToCode, IOCTL_CHANGER_EXCHANGE_MEDIUM);
			INSERT(m_mapCodeAndMnemonic, m_mapMnemonicToCode, IOCTL_CHANGER_MOVE_MEDIUM);
			INSERT(m_mapCodeAndMnemonic, m_mapMnemonicToCode, IOCTL_CHANGER_REINITIALIZE_TRANSPORT);
			INSERT(m_mapCodeAndMnemonic, m_mapMnemonicToCode, IOCTL_CHANGER_QUERY_VOLUME_TAGS);
			INSERT(m_mapCodeAndMnemonic, m_mapMnemonicToCode, IOCTL_SERIAL_LSRMST_INSERT);
			INSERT(m_mapCodeAndMnemonic, m_mapMnemonicToCode, IOCTL_SERENUM_EXPOSE_HARDWARE);
			INSERT(m_mapCodeAndMnemonic, m_mapMnemonicToCode, IOCTL_SERENUM_REMOVE_HARDWARE);
			INSERT(m_mapCodeAndMnemonic, m_mapMnemonicToCode, IOCTL_SERENUM_PORT_DESC);
			INSERT(m_mapCodeAndMnemonic, m_mapMnemonicToCode, IOCTL_SERENUM_GET_PORT_NAME);
			INSERT(m_mapCodeAndMnemonic, m_mapMnemonicToCode, FSCTL_REQUEST_OPLOCK_LEVEL_1);
			INSERT(m_mapCodeAndMnemonic, m_mapMnemonicToCode, FSCTL_REQUEST_OPLOCK_LEVEL_2);
			INSERT(m_mapCodeAndMnemonic, m_mapMnemonicToCode, FSCTL_REQUEST_BATCH_OPLOCK);
			INSERT(m_mapCodeAndMnemonic, m_mapMnemonicToCode, FSCTL_OPLOCK_BREAK_ACKNOWLEDGE);
			INSERT(m_mapCodeAndMnemonic, m_mapMnemonicToCode, FSCTL_OPBATCH_ACK_CLOSE_PENDING);
			INSERT(m_mapCodeAndMnemonic, m_mapMnemonicToCode, FSCTL_OPLOCK_BREAK_NOTIFY);
			INSERT(m_mapCodeAndMnemonic, m_mapMnemonicToCode, FSCTL_LOCK_VOLUME);
			INSERT(m_mapCodeAndMnemonic, m_mapMnemonicToCode, FSCTL_UNLOCK_VOLUME);
			INSERT(m_mapCodeAndMnemonic, m_mapMnemonicToCode, FSCTL_DISMOUNT_VOLUME);
			INSERT(m_mapCodeAndMnemonic, m_mapMnemonicToCode, FSCTL_IS_VOLUME_MOUNTED);
			INSERT(m_mapCodeAndMnemonic, m_mapMnemonicToCode, FSCTL_IS_PATHNAME_VALID);
			INSERT(m_mapCodeAndMnemonic, m_mapMnemonicToCode, FSCTL_MARK_VOLUME_DIRTY);
			INSERT(m_mapCodeAndMnemonic, m_mapMnemonicToCode, FSCTL_QUERY_RETRIEVAL_POINTERS);
			INSERT(m_mapCodeAndMnemonic, m_mapMnemonicToCode, FSCTL_GET_COMPRESSION);
			INSERT(m_mapCodeAndMnemonic, m_mapMnemonicToCode, FSCTL_SET_COMPRESSION);
			INSERT(m_mapCodeAndMnemonic, m_mapMnemonicToCode, FSCTL_SET_BOOTLOADER_ACCESSED);
			INSERT(m_mapCodeAndMnemonic, m_mapMnemonicToCode, FSCTL_OPLOCK_BREAK_ACK_NO_2);
			INSERT(m_mapCodeAndMnemonic, m_mapMnemonicToCode, FSCTL_INVALIDATE_VOLUMES);
			INSERT(m_mapCodeAndMnemonic, m_mapMnemonicToCode, FSCTL_QUERY_FAT_BPB);
			INSERT(m_mapCodeAndMnemonic, m_mapMnemonicToCode, FSCTL_REQUEST_FILTER_OPLOCK);
			INSERT(m_mapCodeAndMnemonic, m_mapMnemonicToCode, FSCTL_FILESYSTEM_GET_STATISTICS);
			INSERT(m_mapCodeAndMnemonic, m_mapMnemonicToCode, FSCTL_GET_NTFS_VOLUME_DATA);
			INSERT(m_mapCodeAndMnemonic, m_mapMnemonicToCode, FSCTL_GET_NTFS_FILE_RECORD);
			INSERT(m_mapCodeAndMnemonic, m_mapMnemonicToCode, FSCTL_GET_VOLUME_BITMAP);
			INSERT(m_mapCodeAndMnemonic, m_mapMnemonicToCode, FSCTL_GET_RETRIEVAL_POINTERS);
			INSERT(m_mapCodeAndMnemonic, m_mapMnemonicToCode, FSCTL_MOVE_FILE);
			INSERT(m_mapCodeAndMnemonic, m_mapMnemonicToCode, FSCTL_IS_VOLUME_DIRTY);
			INSERT(m_mapCodeAndMnemonic, m_mapMnemonicToCode, FSCTL_ALLOW_EXTENDED_DASD_IO);
			INSERT(m_mapCodeAndMnemonic, m_mapMnemonicToCode, FSCTL_FIND_FILES_BY_SID);
			INSERT(m_mapCodeAndMnemonic, m_mapMnemonicToCode, FSCTL_SET_OBJECT_ID);
			INSERT(m_mapCodeAndMnemonic, m_mapMnemonicToCode, FSCTL_GET_OBJECT_ID);
			INSERT(m_mapCodeAndMnemonic, m_mapMnemonicToCode, FSCTL_DELETE_OBJECT_ID);
			INSERT(m_mapCodeAndMnemonic, m_mapMnemonicToCode, FSCTL_SET_REPARSE_POINT);
			INSERT(m_mapCodeAndMnemonic, m_mapMnemonicToCode, FSCTL_GET_REPARSE_POINT);
			INSERT(m_mapCodeAndMnemonic, m_mapMnemonicToCode, FSCTL_DELETE_REPARSE_POINT);
			INSERT(m_mapCodeAndMnemonic, m_mapMnemonicToCode, FSCTL_ENUM_USN_DATA);
			INSERT(m_mapCodeAndMnemonic, m_mapMnemonicToCode, FSCTL_SECURITY_ID_CHECK);
			INSERT(m_mapCodeAndMnemonic, m_mapMnemonicToCode, FSCTL_READ_USN_JOURNAL);
			INSERT(m_mapCodeAndMnemonic, m_mapMnemonicToCode, FSCTL_SET_OBJECT_ID_EXTENDED);
			INSERT(m_mapCodeAndMnemonic, m_mapMnemonicToCode, FSCTL_CREATE_OR_GET_OBJECT_ID);
			INSERT(m_mapCodeAndMnemonic, m_mapMnemonicToCode, FSCTL_SET_SPARSE);
			INSERT(m_mapCodeAndMnemonic, m_mapMnemonicToCode, FSCTL_SET_ZERO_DATA);
			INSERT(m_mapCodeAndMnemonic, m_mapMnemonicToCode, FSCTL_QUERY_ALLOCATED_RANGES);
			INSERT(m_mapCodeAndMnemonic, m_mapMnemonicToCode, FSCTL_ENABLE_UPGRADE);
			INSERT(m_mapCodeAndMnemonic, m_mapMnemonicToCode, FSCTL_SET_ENCRYPTION);
			INSERT(m_mapCodeAndMnemonic, m_mapMnemonicToCode, FSCTL_ENCRYPTION_FSCTL_IO);
			INSERT(m_mapCodeAndMnemonic, m_mapMnemonicToCode, FSCTL_WRITE_RAW_ENCRYPTED);
			INSERT(m_mapCodeAndMnemonic, m_mapMnemonicToCode, FSCTL_READ_RAW_ENCRYPTED);
			INSERT(m_mapCodeAndMnemonic, m_mapMnemonicToCode, FSCTL_CREATE_USN_JOURNAL);
			INSERT(m_mapCodeAndMnemonic, m_mapMnemonicToCode, FSCTL_READ_FILE_USN_DATA);
			INSERT(m_mapCodeAndMnemonic, m_mapMnemonicToCode, FSCTL_WRITE_USN_CLOSE_RECORD);
			INSERT(m_mapCodeAndMnemonic, m_mapMnemonicToCode, FSCTL_EXTEND_VOLUME);
			INSERT(m_mapCodeAndMnemonic, m_mapMnemonicToCode, FSCTL_QUERY_USN_JOURNAL);
			INSERT(m_mapCodeAndMnemonic, m_mapMnemonicToCode, FSCTL_DELETE_USN_JOURNAL);
			INSERT(m_mapCodeAndMnemonic, m_mapMnemonicToCode, FSCTL_MARK_HANDLE);
			INSERT(m_mapCodeAndMnemonic, m_mapMnemonicToCode, FSCTL_SIS_COPYFILE);
			INSERT(m_mapCodeAndMnemonic, m_mapMnemonicToCode, FSCTL_SIS_LINK_FILES);
			INSERT(m_mapCodeAndMnemonic, m_mapMnemonicToCode, FSCTL_RECALL_FILE);
			INSERT(m_mapCodeAndMnemonic, m_mapMnemonicToCode, FSCTL_READ_FROM_PLEX);
			INSERT(m_mapCodeAndMnemonic, m_mapMnemonicToCode, FSCTL_FILE_PREFETCH);
			INSERT(m_mapCodeAndMnemonic, m_mapMnemonicToCode, FSCTL_MAKE_MEDIA_COMPATIBLE);
			INSERT(m_mapCodeAndMnemonic, m_mapMnemonicToCode, FSCTL_SET_DEFECT_MANAGEMENT);
			INSERT(m_mapCodeAndMnemonic, m_mapMnemonicToCode, FSCTL_QUERY_SPARING_INFO);
			INSERT(m_mapCodeAndMnemonic, m_mapMnemonicToCode, FSCTL_QUERY_ON_DISK_VOLUME_INFO);
			INSERT(m_mapCodeAndMnemonic, m_mapMnemonicToCode, FSCTL_SET_VOLUME_COMPRESSION_STATE);
			INSERT(m_mapCodeAndMnemonic, m_mapMnemonicToCode, FSCTL_TXFS_MODIFY_RM);
			INSERT(m_mapCodeAndMnemonic, m_mapMnemonicToCode, FSCTL_TXFS_QUERY_RM_INFORMATION);
			INSERT(m_mapCodeAndMnemonic, m_mapMnemonicToCode, FSCTL_TXFS_ROLLFORWARD_REDO);
			INSERT(m_mapCodeAndMnemonic, m_mapMnemonicToCode, FSCTL_TXFS_ROLLFORWARD_UNDO);
			INSERT(m_mapCodeAndMnemonic, m_mapMnemonicToCode, FSCTL_TXFS_START_RM);
			INSERT(m_mapCodeAndMnemonic, m_mapMnemonicToCode, FSCTL_TXFS_SHUTDOWN_RM);
			INSERT(m_mapCodeAndMnemonic, m_mapMnemonicToCode, FSCTL_TXFS_READ_BACKUP_INFORMATION);
			INSERT(m_mapCodeAndMnemonic, m_mapMnemonicToCode, FSCTL_TXFS_WRITE_BACKUP_INFORMATION);
			INSERT(m_mapCodeAndMnemonic, m_mapMnemonicToCode, FSCTL_TXFS_CREATE_SECONDARY_RM);
			INSERT(m_mapCodeAndMnemonic, m_mapMnemonicToCode, FSCTL_TXFS_GET_METADATA_INFO);
			INSERT(m_mapCodeAndMnemonic, m_mapMnemonicToCode, FSCTL_TXFS_GET_TRANSACTED_VERSION);
			INSERT(m_mapCodeAndMnemonic, m_mapMnemonicToCode, FSCTL_TXFS_SAVEPOINT_INFORMATION);
			INSERT(m_mapCodeAndMnemonic, m_mapMnemonicToCode, FSCTL_TXFS_CREATE_MINIVERSION);
			INSERT(m_mapCodeAndMnemonic, m_mapMnemonicToCode, FSCTL_TXFS_TRANSACTION_ACTIVE);
			INSERT(m_mapCodeAndMnemonic, m_mapMnemonicToCode, FSCTL_SET_ZERO_ON_DEALLOCATION);
			INSERT(m_mapCodeAndMnemonic, m_mapMnemonicToCode, FSCTL_SET_REPAIR);
			INSERT(m_mapCodeAndMnemonic, m_mapMnemonicToCode, FSCTL_GET_REPAIR);
			INSERT(m_mapCodeAndMnemonic, m_mapMnemonicToCode, FSCTL_WAIT_FOR_REPAIR);
			INSERT(m_mapCodeAndMnemonic, m_mapMnemonicToCode, FSCTL_INITIATE_REPAIR);
			INSERT(m_mapCodeAndMnemonic, m_mapMnemonicToCode, FSCTL_CSC_INTERNAL);
			INSERT(m_mapCodeAndMnemonic, m_mapMnemonicToCode, FSCTL_SHRINK_VOLUME);
			INSERT(m_mapCodeAndMnemonic, m_mapMnemonicToCode, FSCTL_SET_SHORT_NAME_BEHAVIOR);
			INSERT(m_mapCodeAndMnemonic, m_mapMnemonicToCode, FSCTL_DFSR_SET_GHOST_HANDLE_STATE);
			INSERT(m_mapCodeAndMnemonic, m_mapMnemonicToCode, FSCTL_TXFS_LIST_TRANSACTIONS);
			INSERT(m_mapCodeAndMnemonic, m_mapMnemonicToCode, FSCTL_QUERY_PAGEFILE_ENCRYPTION);
			INSERT(m_mapCodeAndMnemonic, m_mapMnemonicToCode, FSCTL_RESET_VOLUME_ALLOCATION_HINTS);
			INSERT(m_mapCodeAndMnemonic, m_mapMnemonicToCode, FSCTL_QUERY_DEPENDENT_VOLUME);
			INSERT(m_mapCodeAndMnemonic, m_mapMnemonicToCode, FSCTL_SD_GLOBAL_CHANGE);
			INSERT(m_mapCodeAndMnemonic, m_mapMnemonicToCode, FSCTL_TXFS_READ_BACKUP_INFORMATION2);
			INSERT(m_mapCodeAndMnemonic, m_mapMnemonicToCode, FSCTL_LOOKUP_STREAM_FROM_CLUSTER);
			INSERT(m_mapCodeAndMnemonic, m_mapMnemonicToCode, FSCTL_TXFS_WRITE_BACKUP_INFORMATION2);
			INSERT(m_mapCodeAndMnemonic, m_mapMnemonicToCode, FSCTL_FILE_TYPE_NOTIFICATION);
			INSERT(m_mapCodeAndMnemonic, m_mapMnemonicToCode, FSCTL_GET_BOOT_AREA_INFO);
			INSERT(m_mapCodeAndMnemonic, m_mapMnemonicToCode, FSCTL_GET_RETRIEVAL_POINTER_BASE);
			INSERT(m_mapCodeAndMnemonic, m_mapMnemonicToCode, FSCTL_SET_PERSISTENT_VOLUME_STATE);
			INSERT(m_mapCodeAndMnemonic, m_mapMnemonicToCode, FSCTL_QUERY_PERSISTENT_VOLUME_STATE);
			INSERT(m_mapCodeAndMnemonic, m_mapMnemonicToCode, FSCTL_REQUEST_OPLOCK);
			INSERT(m_mapCodeAndMnemonic, m_mapMnemonicToCode, FSCTL_CSV_TUNNEL_REQUEST);
			INSERT(m_mapCodeAndMnemonic, m_mapMnemonicToCode, FSCTL_IS_CSV_FILE);
			INSERT(m_mapCodeAndMnemonic, m_mapMnemonicToCode, FSCTL_QUERY_FILE_SYSTEM_RECOGNITION);
			INSERT(m_mapCodeAndMnemonic, m_mapMnemonicToCode, FSCTL_CSV_GET_VOLUME_PATH_NAME);
			INSERT(m_mapCodeAndMnemonic, m_mapMnemonicToCode, FSCTL_CSV_GET_VOLUME_NAME_FOR_VOLUME_MOUNT_POINT);
			INSERT(m_mapCodeAndMnemonic, m_mapMnemonicToCode, FSCTL_CSV_GET_VOLUME_PATH_NAMES_FOR_VOLUME_NAME);
			INSERT(m_mapCodeAndMnemonic, m_mapMnemonicToCode, FSCTL_IS_FILE_ON_CSV_VOLUME);
			INSERT(m_mapCodeAndMnemonic, m_mapMnemonicToCode, IOCTL_VOLUME_GET_VOLUME_DISK_EXTENTS);
			INSERT(m_mapCodeAndMnemonic, m_mapMnemonicToCode, IOCTL_VOLUME_ONLINE);
			INSERT(m_mapCodeAndMnemonic, m_mapMnemonicToCode, IOCTL_VOLUME_OFFLINE);
			INSERT(m_mapCodeAndMnemonic, m_mapMnemonicToCode, IOCTL_VOLUME_IS_CLUSTERED);
			INSERT(m_mapCodeAndMnemonic, m_mapMnemonicToCode, IOCTL_VOLUME_GET_GPT_ATTRIBUTES);

		}
	}


#undef INSERT
}

ULONG_PTR CIoctlEdtControl::GetMnemonicNumber()
{
	return m_mapCodeAndMnemonic.size();
}

void CIoctlEdtControl::MnemonicSet(const wxString & strMnemonic)
{
	do
	{
		wxString newStr = strMnemonic.Upper();
		auto iter = m_mapMnemonicToCode.find(newStr);
		if (iter == m_mapMnemonicToCode.cend())
		{
			IOCTL_INFO ioctlInfo = { 0 };
			UpdateIoctlInfo(ioctlInfo);
		}
		else
		{
			IOCTL_INFO ioctlInfo = { iter->second };
			UpdateIoctlInfo(ioctlInfo);
		}
		
	} while (false);
}

void CIoctlEdtControl::GetIoctlInfo(IOCTL_INFO & ioctlInfo)
{
	wxString strCode = m_pEdtCode->GetLabelText();
	_tcscanf_s(strCode.c_str(), TEXT("%x"), &ioctlInfo.ulData);
}

void CIoctlEdtControl::UpdateIoctlInfo(const IOCTL_INFO & ioctlInfo)
{
	do 
	{
		if (ioctlInfo.ulData == 0)
		{
			m_pEdtDevType->SetLabelText(wxT(""));
			m_pEdtFunction->SetLabelText(wxT(""));
			m_pEdtMethod->SetLabelText(wxT(""));
			m_pEdtAccess->SetLabelText(wxT(""));
			m_pEdtLayout->SetLabelText(wxT(""));
			m_pEdtCode->SetInsertionPoint(-1);
			m_pEdtLayout->SetLabelText(wxT("00000000000000000000000000000000"));
			break;;
		}

		wxString strTemp;
		strTemp = wxString::Format(wxT("%08X"), ioctlInfo.ulData);
		m_pEdtCode->SetLabelText(strTemp);
		m_pEdtCode->SetInsertionPoint(-1);

		//DeviceType
		auto p1 = m_mapDeviceType.find(ioctlInfo.Info.DeviceType);
		if (p1 != m_mapDeviceType.end())
		{
			m_pEdtDevType->SetLabelText(p1->second);
		}
		else
		{
			m_pEdtDevType->SetLabelText(wxT(""));
		}

		//Function
		strTemp = wxString::Format(wxT("0x%X"), ioctlInfo.Info.Function);
		m_pEdtFunction->SetLabelText(strTemp);

		//Method
		auto p2 = m_mapMethod.find(ioctlInfo.Info.Method);
		if (p2 != m_mapMethod.end())
		{
			m_pEdtMethod->SetLabelText(p2->second);
		}
		else
		{
			m_pEdtMethod->SetLabelText(wxT(""));
		}

		//Access
		auto p3 = m_mapAccess.find(ioctlInfo.Info.Access);
		if (p3 != m_mapAccess.end())
		{
			m_pEdtAccess->SetLabelText(p3->second);
		}
		else
		{
			m_pEdtAccess->SetLabelText(wxT(""));
		}

		//DeviceType
		auto p4 = m_mapDeviceType.find(ioctlInfo.Info.DeviceType);
		if (p4 != m_mapDeviceType.end())
		{
			m_pEdtDevType->SetLabelText(p4->second);
		}
		else
		{
			m_pEdtDevType->SetLabelText(wxT("0"));
		}

		//Mnemonic
		auto p5 = m_mapCodeAndMnemonic.find(ioctlInfo.ulData);
		if (p5 != m_mapCodeAndMnemonic.end())
		{
			m_pEdtMnemonic->SetLabelText(p5->second);
		}
		else
		{
			m_pEdtMnemonic->SetLabelText(wxT(""));
		}
		std::bitset<32> bitData(ioctlInfo.ulData);
		m_pEdtLayout->SetLabelText(bitData.to_string());
	} while (0);
	
}
