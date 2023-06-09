
#include "pch.h"
#include "RealkassTsd.h"

#ifdef WIN32
#pragma setlocale("ru-RU" )
#endif

static const wchar_t* g_PropNames[] = {
	L"Verison",
	L"Ip"
};
static const wchar_t* g_PropNamesRu[] = {
	L"Версия",
	L"Айпи"
};

static const wchar_t* g_MethodNames[] = {
	L"GetVersion",
	L"GetDescription",
	L"GetLastError",
	L"GetParameters",
	L"SetParameter",
	L"Open",
	L"Close",
	L"DeviceTest",
	L"GetAdditionalActions",
	L"DoAdditionalAction",
	L"UploadTable",
	L"DownloadTable",
	L"ClearTable"
};
static const wchar_t* g_MethodNamesRu[] = {
	L"ПолучитьНомерВерсии",
	L"ПолучитьОписание",
	L"ПолучитьОшибку",
	L"ПолучитьПараметры",
	L"УстановитьПараметр",
	L"Подключить",
	L"Отключить",
	L"ТестУстройства",
	L"ПолучитьДополнительныеДействия",
	L"ВыполнитьДополнительноеДействие",
	L"ВыгрузитьТаблицу",
	L"ЗагрузитьТаблицу",
	L"ОчиститьТаблицу"
};


uint32_t convToShortWchar(WCHAR_T** Dest, const wchar_t* Source, uint32_t len = 0);
uint32_t convFromShortWchar(wchar_t** Dest, const WCHAR_T* Source, uint32_t len = 0);
uint32_t getLenShortWcharStr(const WCHAR_T* Source);

static const wchar_t g_kClassNames[] = L"RealkassTsd";
static const wchar_t* extensionName = L"Realkass";
static const wchar_t* extensionVersion = L"1.0.0.1"; // Мажорная версия, минорная версия, исправления, номер сборки

static AppCapabilities g_capabilities = eAppCapabilitiesInvalid;
static WcharWrapper s_names(g_kClassNames);

HANDLE hTempFile;

TCHAR lpTempPathBuffer[MAX_PATH];

//---------------------------------------------------------------------------//
const WCHAR_T* GetClassNames()
{
	return s_names;
}
//---------------------------------------------------------------------------//
long GetClassObject(const WCHAR_T* wsName, IComponentBase** pInterface)
{
	if (!*pInterface)
	{
		*pInterface = new RealkassTsd;
		return (long)*pInterface;
	}
	return 0;
}
//---------------------------------------------------------------------------//
AppCapabilities SetPlatformCapabilities(const AppCapabilities capabilities)
{
	return g_capabilities;
}
//---------------------------------------------------------------------------//
long DestroyObject(IComponentBase** pIntf)
{
	if (!*pIntf)
		return -1;

	delete* pIntf;
	*pIntf = 0;
	return 0;
}
//---------------------------------------------------------------------------//
// RealkassTsd
//---------------------------------------------------------------------------//
RealkassTsd::RealkassTsd()
{
	m_iMemory = 0;
	m_iConnect = 0;
}
//---------------------------------------------------------------------------//
RealkassTsd::~RealkassTsd()
{
}
//---------------------------------------------------------------------------//
bool RealkassTsd::Init(void* pConnection)
{
	m_iConnect = (IAddInDefBase*)pConnection;
	m_iConnect->SetEventBufferDepth(100);
	return m_iConnect != NULL;
}
//---------------------------------------------------------------------------//
bool RealkassTsd::setMemManager(void* memManager)
{
	m_iMemory = (IMemoryManager*)memManager;
	return m_iMemory != 0;
}
//---------------------------------------------------------------------------//
long RealkassTsd::GetInfo()
{
	return 3003;
}
//---------------------------------------------------------------------------//
void RealkassTsd::Done()
{

}
//---------------------------------------------------------------------------//
bool RealkassTsd::RegisterExtensionAs(WCHAR_T** wsExtensionName)
{
	const wchar_t* wsExtension = extensionName;
	size_t iActualSize = ::wcslen(wsExtension) + 1;
	WCHAR_T* dest = 0;

	if (m_iMemory)
	{
		if (m_iMemory->AllocMemory((void**)wsExtensionName, iActualSize * sizeof(WCHAR_T)))
			::convToShortWchar(wsExtensionName, wsExtension, iActualSize);
		return true;
	}

	return false;
}
//---------------------------------------------------------------------------//
long RealkassTsd::GetNProps()
{
	return epLast;
}
//---------------------------------------------------------------------------//
long RealkassTsd::FindProp(const WCHAR_T* wsPropName)
{
	long plPropNum = -1;
	wchar_t* propName = 0;

	::convFromShortWchar(&propName, wsPropName);
	plPropNum = findName(g_PropNames, propName, epLast);

	if (plPropNum == -1)
		plPropNum = findName(g_PropNamesRu, propName, epLast);

	delete[] propName;

	return plPropNum;
}
//---------------------------------------------------------------------------//
const WCHAR_T* RealkassTsd::GetPropName(long lPropNum, long lPropAlias)
{
	if (lPropNum >= epLast)
		return NULL;

	wchar_t* wsCurrentName = NULL;
	WCHAR_T* wsPropName = NULL;
	int iActualSize = 0;

	switch (lPropAlias)
	{
	case 0: // First language
		wsCurrentName = (wchar_t*)g_PropNames[lPropNum];
		break;
	case 1: // Second language
		wsCurrentName = (wchar_t*)g_PropNamesRu[lPropNum];
		break;
	default:
		return 0;
	}

	iActualSize = wcslen(wsCurrentName) + 1;

	if (m_iMemory && wsCurrentName)
	{
		if (m_iMemory->AllocMemory((void**)&wsPropName, iActualSize * sizeof(WCHAR_T)))
			::convToShortWchar(&wsPropName, wsCurrentName, iActualSize);
	}

	return wsPropName;
}
//---------------------------------------------------------------------------//
bool RealkassTsd::GetPropVal(const long lPropNum, tVariant* pvarPropVal)
{
	switch (lPropNum)
	{
	case epVersion:
	{
		setWStringToTVariant(pvarPropVal, L"1.0.0.1");
		return true;
	}
	default:
		return false;
	}

	return true;
}
//---------------------------------------------------------------------------//
bool RealkassTsd::SetPropVal(const long lPropNum, tVariant* varPropVal)
{
	switch (lPropNum)
	{
	default:
		return false;
	}

	return true;
}
//---------------------------------------------------------------------------//
bool RealkassTsd::IsPropReadable(const long lPropNum)
{
	switch (lPropNum)
	{
	case epVersion:
		return true;
	default:
		return false;
	}

	return false;
}
//---------------------------------------------------------------------------//
bool RealkassTsd::IsPropWritable(const long lPropNum)
{
	switch (lPropNum)
	{
	case epVersion:
		return false;
	default:
		return false;
	}

	return false;
}
//---------------------------------------------------------------------------//
long RealkassTsd::GetNMethods()
{
	return emLast;
}
//---------------------------------------------------------------------------//
long RealkassTsd::FindMethod(const WCHAR_T* wsMethodName)
{
	long plMethodNum = -1;
	wchar_t* name = 0;

	::convFromShortWchar(&name, wsMethodName);

	plMethodNum = findName(g_MethodNames, name, emLast);

	if (plMethodNum == -1)
		plMethodNum = findName(g_MethodNamesRu, name, emLast);

	delete[] name;

	return plMethodNum;
}
//---------------------------------------------------------------------------//
const WCHAR_T* RealkassTsd::GetMethodName(const long lMethodNum, const long lMethodAlias)
{
	if (lMethodNum >= emLast)
		return NULL;

	wchar_t* wsCurrentName = NULL;
	WCHAR_T* wsMethodName = NULL;
	int iActualSize = 0;

	switch (lMethodAlias)
	{
	case 0: // First language
		wsCurrentName = (wchar_t*)g_MethodNames[lMethodNum];
		break;
	case 1: // Second language
		wsCurrentName = (wchar_t*)g_MethodNamesRu[lMethodNum];
		break;
	default:
		return 0;
	}

	iActualSize = wcslen(wsCurrentName) + 1;

	if (m_iMemory && wsCurrentName)
	{
		if (m_iMemory->AllocMemory((void**)&wsMethodName, iActualSize * sizeof(WCHAR_T)))
			::convToShortWchar(&wsMethodName, wsCurrentName, iActualSize);
	}

	return wsMethodName;
}
//---------------------------------------------------------------------------//
long RealkassTsd::GetNParams(const long lMethodNum)
{
	switch (lMethodNum)
	{
	case emGetVersion:
		return 0;
	case emGetDescription:
		return 7;
	case emGetLastError:
		return 1;
	case emGetParameters:
		return 1;
	case emSetParameter:
		return 2;
	case emOpen:
		return 1;
	case emClose:
		return 1;
	case emDeviceTest:
		return 2;
	case emGetAdditionalActions:
		return 1;
	case emDoAdditionalAction:
		return 1;
	case emUploadTable:
		return 3;
	case emDownloadTable:
		return 2;
	case emClearTable:
		return 1;
	default:
		return 0;
	}

	return 0;
}
//---------------------------------------------------------------------------//
bool RealkassTsd::GetParamDefValue(const long lMethodNum, const long lParamNum, tVariant* pvarParamDefValue)
{
	TV_VT(pvarParamDefValue) = VTYPE_EMPTY;

	switch (lMethodNum)
	{
	default:
		return false;
	}

	return false;
}
//---------------------------------------------------------------------------//
bool RealkassTsd::HasRetVal(const long lMethodNum)
{
	switch (lMethodNum)
	{
	case emGetVersion:
		return true;
	case emGetDescription:
		return true;
	case emGetLastError:
		return true;
	case emGetParameters:
		return true;
	case emSetParameter:
		return true;
	case emOpen:
		return true;
	case emClose:
		return true;
	case emDeviceTest:
		return true;
	case emGetAdditionalActions:
		return true;
	case emDoAdditionalAction:
		return true;
	case emUploadTable:
		return true;
	case emDownloadTable:
		return true;
	case emClearTable:
		return true;
	default:
		return false;
	}

	return false;
}
//---------------------------------------------------------------------------//
bool RealkassTsd::CallAsProc(const long lMethodNum, tVariant* paParams, const long lSizeArray)
{
	switch (lMethodNum)
	{
	default:
		return false;
	}

	return true;
}
//---------------------------------------------------------------------------//

bool RealkassTsd::CallAsFunc(const long lMethodNum, tVariant* pvarRetValue, tVariant* paParams, const long lSizeArray)
{
	switch (lMethodNum)
	{
		// Method acceps one argument of type BinaryData ant returns its copy
	case emGetVersion:
		return mGetVersion(pvarRetValue);
	case emGetDescription:
		return mGetDescription(pvarRetValue, paParams, &paParams[1], &paParams[2], &paParams[3], &paParams[4], &paParams[5], &paParams[6]);
	case emGetLastError:
		return mGetLastError(pvarRetValue, paParams);
	case emGetParameters:
		return mGetParameters(pvarRetValue, paParams);
	case emSetParameter:
		return mSetParameter(pvarRetValue, paParams, &paParams[1]);
	case emOpen:
		return mOpen(pvarRetValue, paParams);
	case emClose:
		return mClose(pvarRetValue, paParams);
	case emDeviceTest:
		return mDeviceTest(pvarRetValue, paParams, &paParams[1]);
	case emGetAdditionalActions:
		return mGetAdditionalActions(pvarRetValue, paParams);
	case emDoAdditionalAction:
		return mDoAdditionalAction(pvarRetValue, paParams);
	case emUploadTable:
		return mUploadTable(pvarRetValue, paParams, &paParams[1], &paParams[2]);
	case emDownloadTable:
		return mDownloadTable(pvarRetValue, paParams, &paParams[1]);
	case emClearTable:
		return mClearTable(pvarRetValue, paParams);
	default:
		return false;
	}

	return false;
}
//---------------------------------------------------------------------------//
void RealkassTsd::SetLocale(const WCHAR_T* loc)
{
#if !defined( __linux__ ) && !defined(__APPLE__)
	_wsetlocale(LC_ALL, loc);
#endif
}
//---------------------------------------------------------------------------//
void RealkassTsd::SetUserInterfaceLanguageCode(const WCHAR_T* wsLanguageCode)
{

}
//---------------------------------------------------------------------------//
void RealkassTsd::addError(uint32_t wcode, const wchar_t* source, const wchar_t* descriptor, long code)
{
	if (m_iConnect)
	{
		WCHAR_T* err = 0;
		WCHAR_T* descr = 0;

		::convToShortWchar(&err, source);
		::convToShortWchar(&descr, descriptor);

		m_iConnect->AddError(wcode, err, descr, code);
		delete[] err;
		delete[] descr;
	}
}
//---------------------------------------------------------------------------//
long RealkassTsd::findName(const wchar_t* names[], const wchar_t* name, const uint32_t size) const
{
	long ret = -1;
	for (uint32_t i = 0; i < size; i++)
	{
		if (!wcscmp(names[i], name))
		{
			ret = i;
			break;
		}
	}
	return ret;
}
//---------------------------------------------------------------------------//
void RealkassTsd::setWStringToTVariant(tVariant* dest, const wchar_t* source) {

	size_t len = ::wcslen(source) + 1;

	TV_VT(dest) = VTYPE_PWSTR;

	if (m_iMemory->AllocMemory((void**)&dest->pwstrVal, len * sizeof(WCHAR_T)))
		convToShortWchar(&dest->pwstrVal, source, len);
	dest->wstrLen = ::wcslen(source);
}
//---------------------------------------------------------------------------//
//---------------------------------------------------------------------------//
bool RealkassTsd::mGetVersion(tVariant* retVal)
{
	if (retVal) {
		setWStringToTVariant(retVal, L"1.0.0.1");
		return true;
	}
	return false;
}
//---------------------------------------------------------------------------//
bool RealkassTsd::mGetDescription(tVariant* retVal, tVariant* name, tVariant* description, tVariant* equipmentType,
	tVariant* interfaceRevision, tVariant* integrationLibrary, tVariant* mainDriverInstalled, tVariant* getDownloadURL)
{
	TV_VT(interfaceRevision) = VTYPE_I4;
	TV_VT(integrationLibrary) = VTYPE_BOOL;
	TV_VT(mainDriverInstalled) = VTYPE_BOOL;

	setWStringToTVariant(name, L"Реалкасс ТСД");
	setWStringToTVariant(description, L"Описание Реалкасс ТСД");
	// Тип оборудования
	//СканерШтрихкода, СчитывательМагнитныхКарт, ФискальныйРегистратор, ПринтерЧеков, ПринтерЭтикеток, 
	//ДисплейПокупателя, ТерминалСбораДанных, ЭквайринговыйТерминал, ЭлектронныеВесы, ВесыСПечатьюЭтикеток, СчитывательRFID, ККТ. 
	setWStringToTVariant(equipmentType, L"ТерминалСбораДанных");

	TV_I4(interfaceRevision) = 3003;
	TV_BOOL(integrationLibrary) = false;
	TV_BOOL(mainDriverInstalled) = true;
	setWStringToTVariant(getDownloadURL, L"");

	if (retVal) {
		TV_VT(retVal) = VTYPE_BOOL;
		TV_BOOL(retVal) = true;
		return true;
	}
	return false;
}
//---------------------------------------------------------------------------//
bool RealkassTsd::mGetLastError(tVariant* retVal, tVariant* retDescription)
{
	setWStringToTVariant(retDescription, L"Возвращаем последнюю ошибку");

	if (retVal) {
		TV_VT(retVal) = VTYPE_I4;
		TV_I4(retVal) = 0;
		return true;
	}

	return false;
}
//---------------------------------------------------------------------------//
bool RealkassTsd::mGetParameters(tVariant* retVal, tVariant* xml)
{
	setWStringToTVariant(xml, L"<?xml version=\"1.0\" encoding=\"UTF-8\" ?>"
		"<Settings>"
		"<Page Caption=\"Параметры\">"
		"<Group Caption=\"Параметры подключения\">"
		"<Parameter Name=\"ip\" Caption=\"Айпи\" Description=\"Description\" TypeValue=\"String\" />"
		"</Group>"
		"</Page>"
		"</Settings>"
	);

	if (retVal) {
		TV_VT(retVal) = VTYPE_BOOL;
		TV_BOOL(retVal) = true;

		return true;
	}
	return false;
}
//---------------------------------------------------------------------------//
bool RealkassTsd::mSetParameter(tVariant* retVal, tVariant* par, tVariant* val)
{
	wchar_t* wPar = 0;

	const wchar_t* temp;

	if (TV_VT(par) == VTYPE_PWSTR)
	{
		convFromShortWchar(&wPar, TV_WSTR(par));

		if (wcscmp(L"ip", wPar) == 0) {

		}
		else if (wcscmp(L"EquipmentType", wPar) == 0) {

		}
		else {
			temp = L"111";
		}
	}
	else
	{
		return false;
	}

	if (retVal) {
		TV_VT(retVal) = VTYPE_BOOL;
		TV_BOOL(retVal) = true;
		return true;
	}

	return false;

}
//---------------------------------------------------------------------------//
bool RealkassTsd::mOpen(tVariant* retVal, tVariant* par)
{
	wchar_t* wPar = 0;

	if (TV_VT(par) == VTYPE_PWSTR)
	{
		convFromShortWchar(&wPar, TV_WSTR(par));
	}
	else
	{
		return false;
	}

	if (retVal) {
		TV_VT(retVal) = VTYPE_BOOL;
		TV_BOOL(retVal) = true;

		return true;
	}

	return false;
}
//---------------------------------------------------------------------------//
bool RealkassTsd::mClose(tVariant* retVal, tVariant* par)
{
	wchar_t* wPar = 0;

	if (TV_VT(par) == VTYPE_PWSTR)
	{
		convFromShortWchar(&wPar, TV_WSTR(par));
	}
	else
	{
		return false;
	}

	if (retVal) {
		TV_VT(retVal) = VTYPE_BOOL;
		TV_BOOL(retVal) = true;
		return true;
	}

	return false;
}
//---------------------------------------------------------------------------//
bool RealkassTsd::mDeviceTest(tVariant* retVal, tVariant* description, tVariant* demoModeIsActivated)
{
	if (retVal) {
		TV_VT(retVal) = VTYPE_BOOL;
		TV_BOOL(retVal) = true;
		return true;
	}
	return false;
}
//---------------------------------------------------------------------------//
bool RealkassTsd::mGetAdditionalActions(tVariant* retVal, tVariant* xml)
{
	setWStringToTVariant(xml, L"<?xml version=\"1.0\" encoding=\"UTF-8\" ?>"
		"<Actions>"
		"<Action Name=\"emGetLastError\" Caption=\"Параметры журналирования111\" />"
		"</Actions>");
	if (retVal) {
		TV_VT(retVal) = VTYPE_BOOL;
		TV_BOOL(retVal) = true;
		return true;
	}
	return false;
}
//---------------------------------------------------------------------------//
bool RealkassTsd::mDoAdditionalAction(tVariant* retVal, tVariant* actionName)
{
	if (retVal) {
		TV_VT(retVal) = VTYPE_BOOL;
		TV_BOOL(retVal) = true;
		return true;
	}
	return false;
}
//---------------------------------------------------------------------------//
bool RealkassTsd::mUploadTable(tVariant* retVal, tVariant* devId, tVariant* xml, tVariant* status)
{
	if (retVal) {
		TV_VT(retVal) = VTYPE_BOOL;
		TV_BOOL(retVal) = true;
		return true;
	}
	return false;
}
//---------------------------------------------------------------------------//
bool RealkassTsd::mDownloadTable(tVariant* retVal, tVariant* devId, tVariant* xml)
{
	if (retVal) {
		TV_VT(retVal) = VTYPE_BOOL;
		TV_BOOL(retVal) = true;
		return true;
	}
	return false;
}
//---------------------------------------------------------------------------//
bool RealkassTsd::mClearTable(tVariant* retVal, tVariant* data)
{
	if (retVal) {
		TV_VT(retVal) = VTYPE_BOOL;
		TV_BOOL(retVal) = true;
		return true;
	}
	return false;
}
//---------------------------------------------------------------------------//
/*void RealkassTsd::write_log(char* OUTBUFFER, int l, char log_type)
{
	char	TMPBUFFER[50];
	DWORD   bytes_written = 0;
	int		bStatus;

	if (m_loging)
	{
		time_t t = time(0);   // get time now
		struct tm* now = localtime(&t);

		sprintf(TMPBUFFER, "[%04d%02d%02d%02d%02d%02d]", now->tm_year + 1900, now->tm_mon + 1, now->tm_mday, now->tm_hour, now->tm_min, now->tm_sec);
		TMPBUFFER[16] = log_type;
		TMPBUFFER[17] = ':';
		bStatus = WriteFile(hTempFile, &TMPBUFFER, 18, &bytes_written, NULL);

		bStatus = WriteFile(hTempFile, OUTBUFFER, l, &bytes_written, NULL);

		TMPBUFFER[0] = '\r';
		TMPBUFFER[1] = '\n';
		bStatus = WriteFile(hTempFile, &TMPBUFFER, 2, &bytes_written, NULL);

	}
}*/
//---------------------------------------------------------------------------//
uint32_t convToShortWchar(WCHAR_T** Dest, const wchar_t* Source, uint32_t len)
{
	if (!len)
		len = ::wcslen(Source) + 1;

	if (!*Dest)
		*Dest = new WCHAR_T[len];

	WCHAR_T* tmpShort = *Dest;
	wchar_t* tmpWChar = (wchar_t*)Source;
	uint32_t res = 0;

	::memset(*Dest, 0, len * sizeof(WCHAR_T));
#ifdef __linux__
	size_t succeed = (size_t)-1;
	size_t f = len * sizeof(wchar_t), t = len * sizeof(WCHAR_T);
	const char* fromCode = sizeof(wchar_t) == 2 ? "UTF-16" : "UTF-32";
	iconv_t cd = iconv_open("UTF-16LE", fromCode);
	if (cd != (iconv_t)-1)
	{
		succeed = iconv(cd, (char**)&tmpWChar, &f, (char**)&tmpShort, &t);
		iconv_close(cd);
		if (succeed != (size_t)-1)
			return (uint32_t)succeed;
	}
#endif //__linux__
	for (; len; --len, ++res, ++tmpWChar, ++tmpShort)
	{
		*tmpShort = (WCHAR_T)*tmpWChar;
	}

	return res;
}
//---------------------------------------------------------------------------//
uint32_t convFromShortWchar(wchar_t** Dest, const WCHAR_T* Source, uint32_t len)
{
	if (!len)
		len = getLenShortWcharStr(Source) + 1;

	if (!*Dest)
		*Dest = new wchar_t[len];

	wchar_t* tmpWChar = *Dest;
	WCHAR_T* tmpShort = (WCHAR_T*)Source;
	uint32_t res = 0;

	::memset(*Dest, 0, len * sizeof(wchar_t));
#ifdef __linux__
	size_t succeed = (size_t)-1;
	const char* fromCode = sizeof(wchar_t) == 2 ? "UTF-16" : "UTF-32";
	size_t f = len * sizeof(WCHAR_T), t = len * sizeof(wchar_t);
	iconv_t cd = iconv_open("UTF-32LE", fromCode);
	if (cd != (iconv_t)-1)
	{
		succeed = iconv(cd, (char**)&tmpShort, &f, (char**)&tmpWChar, &t);
		iconv_close(cd);
		if (succeed != (size_t)-1)
			return (uint32_t)succeed;
	}
#endif //__linux__
	for (; len; --len, ++res, ++tmpWChar, ++tmpShort)
	{
		*tmpWChar = (wchar_t)*tmpShort;
	}

	return res;
}
//---------------------------------------------------------------------------//
uint32_t getLenShortWcharStr(const WCHAR_T* Source)
{
	uint32_t res = 0;
	WCHAR_T* tmpShort = (WCHAR_T*)Source;

	while (*tmpShort++)
		++res;

	return res;
}
//---------------------------------------------------------------------------//
WcharWrapper::WcharWrapper(const wchar_t* str) :
#ifdef LINUX_OR_MACOS
	m_str_WCHAR(NULL),
#endif 
	m_str_wchar(NULL)
{
	if (str)
	{
		int len = wcslen(str);
		m_str_wchar = new wchar_t[len + 1];
		memset(m_str_wchar, 0, sizeof(wchar_t) * (len + 1));
		memcpy(m_str_wchar, str, sizeof(wchar_t) * len);
#ifdef LINUX_OR_MACOS
		::convToShortWchar(&m_str_WCHAR, m_str_wchar);
#endif
	}

}
//---------------------------------------------------------------------------//
WcharWrapper::~WcharWrapper()
{
#ifdef LINUX_OR_MACOS
	if (m_str_WCHAR)
	{
		delete[] m_str_WCHAR;
		m_str_WCHAR = NULL;
	}
#endif
	if (m_str_wchar)
	{
		delete[] m_str_wchar;
		m_str_wchar = NULL;
	}
}
//---------------------------------------------------------------------------//
