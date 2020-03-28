
#include "innNative.h"

#ifdef WIN32
#pragma setlocale("ru-RU" )
#endif

static const wchar_t *g_PropNames[] = {
    L"Verison"
};
static const wchar_t *g_PropNamesRu[] = {
	L"Версия"
};

static const wchar_t *g_MethodNames[] = {
    L"GetVersion", 
    L"GetDescription", 
    L"GetLastError",
    L"GetParameters", 
    L"SetParameter", 
    L"Open", 
    L"Close", 
    L"DeviceTest",
	L"GetAdditionalActions",
	L"DoAdditionalAction"
};
static const wchar_t *g_MethodNamesRu[] = {
	L"ПолучитьНомерВерсии",
	L"ПолучитьОписание",
	L"ПолучитьОшибку",
	L"ПолучитьПараметры",
	L"УстановитьПараметр",
	L"Подключить",
	L"Отключить",
	L"ТестУстройства",
	L"ПолучитьДополнительныеДействия",
	L"ВыполнитьДополнительноеДействие"
};

static const wchar_t g_kClassNames[] = L"innNative"; 
static const wchar_t* extensionName = L"InnovaIT"; 
static const wchar_t* extensionVersion = L"1.0.0.0"; // Мажорная версия, минорная версия, исправления, номер сборки
static IAddInDefBase *pAsyncEvent = NULL;

uint32_t convToShortWchar(WCHAR_T** Dest, const wchar_t* Source, uint32_t len = 0);
uint32_t convFromShortWchar(wchar_t** Dest, const WCHAR_T* Source, uint32_t len = 0);
uint32_t getLenShortWcharStr(const WCHAR_T* Source);
static AppCapabilities g_capabilities = eAppCapabilitiesInvalid;
static WcharWrapper s_names(g_kClassNames);

char* WCHAR_2_CHAR(wchar_t *in_str);
wchar_t* CHAR_2_WCHAR(char *in_str);

//---------------------------------------------------------------------------//
long GetClassObject(const WCHAR_T* wsName, IComponentBase** pInterface)
{
    if(!*pInterface)
    {
        *pInterface= new innNative;
        return (long)*pInterface;
    }
    return 0;
}
//---------------------------------------------------------------------------//
AppCapabilities SetPlatformCapabilities(const AppCapabilities capabilities)
{
    g_capabilities = capabilities;
    return eAppCapabilitiesLast;
}
//---------------------------------------------------------------------------//
long DestroyObject(IComponentBase** pIntf)
{
    if(!*pIntf)
        return -1;

    delete *pIntf;
    *pIntf = 0;
    return 0;
}
//---------------------------------------------------------------------------//
const WCHAR_T* GetClassNames()
{
    return s_names;
}
//---------------------------------------------------------------------------//
// innNative
//---------------------------------------------------------------------------//
innNative::innNative()
{
    m_iMemory = 0;
    m_iConnect = 0;
}
//---------------------------------------------------------------------------//
innNative::~innNative()
{
}
//---------------------------------------------------------------------------//
bool innNative::Init(void* pConnection)
{ 
    m_iConnect = (IAddInDefBase*)pConnection;
    m_iConnect->SetEventBufferDepth(100);
    return m_iConnect != NULL;
}
//---------------------------------------------------------------------------//
long innNative::GetInfo()
{ 
    // Component should put supported component technology version 
    // This component supports 2.0 version
    return 2000; 
}
//---------------------------------------------------------------------------//
void innNative::Done()
{

}
/////////////////////////////////////////////////////////////////////////////
// ILanguageExtenderBase
//---------------------------------------------------------------------------//
bool innNative::RegisterExtensionAs(WCHAR_T** wsExtensionName)
{ 
	const wchar_t *wsExtension = extensionName;
    size_t iActualSize = ::wcslen(wsExtension) + 1;
    WCHAR_T* dest = 0;

    if (m_iMemory)
    {
        if(m_iMemory->AllocMemory((void**)wsExtensionName, iActualSize * sizeof(WCHAR_T)))
            ::convToShortWchar(wsExtensionName, wsExtension, iActualSize);
        return true;
    }

    return false; 
}
//---------------------------------------------------------------------------//
long innNative::GetNProps()
{ 
    return epLast;
}
//---------------------------------------------------------------------------//
long innNative::FindProp(const WCHAR_T* wsPropName)
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
const WCHAR_T* innNative::GetPropName(long lPropNum, long lPropAlias)
{ 
    if (lPropNum >= epLast)
        return NULL;

    wchar_t *wsCurrentName = NULL;
    WCHAR_T *wsPropName = NULL;
    int iActualSize = 0;

    switch(lPropAlias)
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
bool innNative::GetPropVal(const long lPropNum, tVariant* pvarPropVal)
{ 
    switch(lPropNum)
    {
    case epVersion:	
	{
		setWStringToTVariant(pvarPropVal, extensionVersion);
		return true;
	}
    default:
        return false;
    }

    return true;
}
//---------------------------------------------------------------------------//
bool innNative::SetPropVal(const long lPropNum, tVariant *varPropVal)
{ 
    switch(lPropNum)
    { 
    default:
        return false;
    }

    return true;
}
//---------------------------------------------------------------------------//
bool innNative::IsPropReadable(const long lPropNum)
{ 
    switch(lPropNum)
    { 
    case epVersion:
        return true;
    default:
        return false;
    }

    return false;
}
//---------------------------------------------------------------------------//
bool innNative::IsPropWritable(const long lPropNum)
{
    switch(lPropNum)
    { 
    case epVersion:
        return false;
    default:
        return false;
    }

    return false;
}
//---------------------------------------------------------------------------//
long innNative::GetNMethods()
{ 
    return emLast;
}
//---------------------------------------------------------------------------//
long innNative::FindMethod(const WCHAR_T* wsMethodName)
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
const WCHAR_T* innNative::GetMethodName(const long lMethodNum, const long lMethodAlias)
{ 
    if (lMethodNum >= emLast)
        return NULL;

    wchar_t *wsCurrentName = NULL;
    WCHAR_T *wsMethodName = NULL;
    int iActualSize = 0;

    switch(lMethodAlias)
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
        if(m_iMemory->AllocMemory((void**)&wsMethodName, iActualSize * sizeof(WCHAR_T)))
            ::convToShortWchar(&wsMethodName, wsCurrentName, iActualSize);
    }

    return wsMethodName;
}
//---------------------------------------------------------------------------//
long innNative::GetNParams(const long lMethodNum)
{ 
    switch(lMethodNum)
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
    default:
        return 0;
    }
    
    return 0;
}
//---------------------------------------------------------------------------//
bool innNative::GetParamDefValue(const long lMethodNum, const long lParamNum,  tVariant *pvarParamDefValue)
{ 
    TV_VT(pvarParamDefValue)= VTYPE_EMPTY;

    switch(lMethodNum)
    { 
    default:
        return false;
    }

    return false;
} 
//---------------------------------------------------------------------------//
bool innNative::HasRetVal(const long lMethodNum)
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
	default:
		return false;
	}

	return false;
}
//---------------------------------------------------------------------------//
bool innNative::CallAsProc(const long lMethodNum, tVariant* paParams, const long lSizeArray)
{ 
	switch (lMethodNum)
	{
	default:
		return false;
	}
    return true;
}
//---------------------------------------------------------------------------//
void innNative::setWStringToTVariant(tVariant *dest, const wchar_t* source) {
	
	size_t len = ::wcslen(source)+1;

	TV_VT(dest) = VTYPE_PWSTR;

	if (m_iMemory->AllocMemory((void**)&dest->pwstrVal, len * sizeof(WCHAR_T)))
	convToShortWchar(&dest->pwstrVal, source, len);
	dest->wstrLen = ::wcslen(source);
}
//---------------------------------------------------------------------------//
bool innNative::mGetVersion(tVariant* retVal)
{
	if (retVal) {
		setWStringToTVariant(retVal, extensionVersion);
		return true;
	}
	return false;
}
//---------------------------------------------------------------------------//
bool innNative::mGetDescription(tVariant* retVal, tVariant* name, tVariant* description, tVariant* equipmentType,
	tVariant* interfaceRevision, tVariant* integrationLibrary, tVariant* mainDriverInstalled, tVariant* getDownloadURL)
{
	TV_VT(interfaceRevision) = VTYPE_I4;
	TV_VT(integrationLibrary) = VTYPE_BOOL;
	TV_VT(mainDriverInstalled) = VTYPE_BOOL;

	setWStringToTVariant(name, L"Наименование драйвера");
	setWStringToTVariant(description, L"Описание драйвера");
	// Тип оборудования
	//СканерШтрихкода, СчитывательМагнитныхКарт, ФискальныйРегистратор, ПринтерЧеков, ПринтерЭтикеток, 
	//ДисплейПокупателя, ТерминалСбораДанных, ЭквайринговыйТерминал, ЭлектронныеВесы, ВесыСПечатьюЭтикеток, СчитывательRFID, ККТ. 
	setWStringToTVariant(equipmentType, L"СканерШтрихкода"); 
	
	TV_I4(interfaceRevision) = 2004;
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
bool innNative::mGetLastError(tVariant* retVal, tVariant* retDescription)
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
bool innNative::mGetParameters(tVariant* retVal, tVariant* xml)
{
	setWStringToTVariant(xml, L"<?xml version=\"1.0\" encoding=\"UTF-8\" ?>"
		"<Settings>"
		"<Page Caption=\"Параметры\">"
		"<Group Caption=\"Параметры подключения\">"
		"<Parameter Name=\"Par1\" Caption=\"Параметр1\" Description=\"Описание в выпадающей подсказке\" TypeValue=\"String\" DefaultValue=\"ЗначениеПоУмолчанию\" />"
		"<Parameter Name=\"Par2\" Caption=\"Параметр2\" Description=\"Описание в выпадающей подсказке\" TypeValue=\"Number\" DefaultValue=\"0\">"
		"<Parameter Name=\"Par3\" Caption=\"Параметр3\" Description=\"Описание в выпадающей подсказке\" TypeValue=\"Boolean\" DefaultValue=\"True\">"
		"<ChoiceList>"
		"<Item Value=\"0\">0</Item>"
		"<Item Value=\"1\">1</Item>"
		"<Item Value=\"2\">2</Item>"
		"<Item Value=\"3\">3</Item>"
		"<Item Value=\"4\">4</Item>"
		"</ChoiceList>"
		"</Parameter>"
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
bool innNative::mSetParameter(tVariant* retVal, tVariant* par, tVariant* val)
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
bool innNative::mOpen(tVariant* retVal, tVariant* par)
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

	receiveInThread = true;
	bool res;

	#if defined( __linux__ )
		res = openThreadLinux(wPar);
	#endif

	#ifdef _WINDOWS
		res = openThreadWindows(wPar);
	#endif

	if (retVal) {
		TV_VT(retVal) = VTYPE_BOOL;
		TV_BOOL(retVal) = res;

		return res;
	}

	return false;
}
//---------------------------------------------------------------------------//
bool innNative::mClose(tVariant* retVal, tVariant* par)
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

	receiveInThread = false;
	bool res;

	#if defined( __linux__ )
		res = closeThreadLinux(wPar);
	#endif

	#ifdef _WINDOWS
		res = closeThreadWindows(wPar);
	#endif

	if (retVal) {
		TV_VT(retVal) = VTYPE_BOOL;
		TV_BOOL(retVal) = res;
		return res;
	}
	
	return false;
}
//---------------------------------------------------------------------------//
bool innNative::mDeviceTest(tVariant* retVal, tVariant* description, tVariant* demoModeIsActivated)
{
	if (retVal) {
		TV_VT(retVal) = VTYPE_BOOL;
		TV_BOOL(retVal) = true;
		return true;
	}
	return false;
}
//---------------------------------------------------------------------------//
bool innNative::mGetAdditionalActions(tVariant* retVal, tVariant* xml)
{
	setWStringToTVariant(xml, L"<?xml version=\"1.0\" encoding=\"UTF-8\" ?>"
		"<Actions>"
			"<Action Name = \"Action1\" Caption = \"Дополнительное действие 1\" / >"
		"</Actions>"
	);
	if (retVal) {
		TV_VT(retVal) = VTYPE_BOOL;
		TV_BOOL(retVal) = true;
		return true;
	}
	return false;
}
//---------------------------------------------------------------------------//
bool innNative::mDoAdditionalAction(tVariant* retVal, tVariant* actionName)
{
	if (retVal) {
		TV_VT(retVal) = VTYPE_BOOL;
		TV_BOOL(retVal) = true;
		return true;
	}
	return false;
}
//---------------------------------------------------------------------------//

bool innNative::CallAsFunc(const long lMethodNum,  tVariant* pvarRetValue, tVariant* paParams, const long lSizeArray)
{ 
 

    switch(lMethodNum)
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
		default:
			return false;
		}
    return false; 
}
//---------------------------------------------------------------------------//
void innNative::SetLocale(const WCHAR_T* loc)
{
#if !defined( __linux__ ) && !defined(__APPLE__)
    _wsetlocale(LC_ALL, loc);
#endif
}
/////////////////////////////////////////////////////////////////////////////
// LocaleBase
//---------------------------------------------------------------------------//
bool innNative::setMemManager(void* mem)
{
    m_iMemory = (IMemoryManager*)mem;
    return m_iMemory != 0;
}
//---------------------------------------------------------------------------//
void innNative::addError(uint32_t wcode, const wchar_t* source, const wchar_t* descriptor, long code)
{
    if (m_iConnect)
    {
        WCHAR_T *err = 0;
        WCHAR_T *descr = 0;
        
        ::convToShortWchar(&err, source);
        ::convToShortWchar(&descr, descriptor);

        m_iConnect->AddError(wcode, err, descr, code);
        delete[] err;
        delete[] descr;
    }
}
//---------------------------------------------------------------------------//
long innNative::findName(const wchar_t* names[], const wchar_t* name, const uint32_t size) const
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

bool innNative::sendEvent(wchar_t* msg, wchar_t* data)
{

    if (m_iConnect)
    {
        WCHAR_T *who = 0, *what = 0, *wdata = 0;
        ::convToShortWchar(&who, extensionName);
        ::convToShortWchar(&what, msg);
        ::convToShortWchar(&wdata, data);

        bool res = m_iConnect->ExternalEvent(who, what, wdata);

        delete[] who;
        delete[] what;
        delete[] wdata;

        return res;
    }
    return false;
}
//---------------------------------------------------------------------------//
uint32_t convToShortWchar(WCHAR_T** Dest, const wchar_t* Source, uint32_t len)
{
    if (!len)
        len = ::wcslen(Source) + 1;

    if (!*Dest)
        *Dest = new WCHAR_T[len];

    WCHAR_T* tmpShort = *Dest;
    wchar_t* tmpWChar = (wchar_t*) Source;
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
        if(succeed != (size_t)-1)
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
        if(succeed != (size_t)-1)
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
    WCHAR_T *tmpShort = (WCHAR_T*)Source;

    while (*tmpShort++)
        ++res;

    return res;
}
//---------------------------------------------------------------------------//

// --------------------------------------------------------------Innova-IT ------------------------------------------------------------------------//
char* WCHAR_2_CHAR(wchar_t *in_str)
{
	size_t len = wcslen(in_str) / sizeof(char) + 1;
	char* out_str = new char[len];
	size_t* out_str_len = 0;
	wcstombs(out_str, in_str, len);

	delete &len;
	delete out_str_len;

	return out_str;
}

// --------------------------------------------------------------Innova-IT ------------------------------------------------------------------------//
wchar_t* CHAR_2_WCHAR(char *in_str)
{
	size_t len = strlen(in_str) + 1;
	wchar_t* out_str = new wchar_t[len * sizeof(wchar_t)];

	size_t* out_str_len = 0;

	mbstowcs(out_str, in_str,len);

	return out_str;
}

//----------------------------------------------------------Процедуры Потоков-----------------------------------------------------------------------//
#ifdef _WINDOWS
static unsigned int _stdcall threadWindows(void*p)
{

	innNative *serialCl = (innNative*)p;

	while (true) {
		Sleep(3000);

		if (!serialCl->receiveInThread) {
			serialCl->sendEvent(L"НовыеДанные", L"Стоп");
			return true;
		}

		serialCl->sendEvent(L"НовыеДанные", L"Тик");

	}

	return true;
}
//---------------------------------------------------------------------------//
bool innNative::openThreadWindows(wchar_t* par)
{
	
	hTh = 0;
	hTh = (HANDLE)_beginthreadex(NULL, 10, threadWindows, (LPVOID)this, 0, &thID);

	if (hTh == 0) {
		sendEvent(L"Ошибка", L"Не удалось создать новый поток");
		return false;
	}

	return true;
}
bool innNative::closeThreadWindows(wchar_t* par)
{

	if (hTh)
	{
		DWORD stopThRes = WaitForSingleObject(hTh, 200);
		hTh = 0;
		if (stopThRes != WAIT_OBJECT_0)
			return false;
	}

	return true;
}
#endif

#if defined( __linux__ )

void* threadLinux(void* p) {
	
	innNative *serialCl = (innNative*)p;

	while (true) {
        usleep(3000000);

		if (!serialCl->receiveInThread) {
            serialCl->sendEvent(L"НовыеДанные", L"Стоп");
			pthread_exit(0);
		}

        serialCl->sendEvent(L"НовыеДанные", L"Тик");
	}

	pthread_exit(0);
}

bool  innNative::openThreadLinux(wchar_t* par)
{

	if (pthread_create(&thID, NULL, threadLinux, (LPVOID)this) != 0) {
		sendEvent(L"Ошибка", L"Не удалось создать новый поток");
		return false;
	}

	return true;
}

bool innNative::closeThreadLinux(wchar_t* par)
{	

	pthread_join(thID, NULL);

	return true;
}
#endif // __LINE__

#ifdef LINUX_OR_MACOS
WcharWrapper::WcharWrapper(const WCHAR_T* str) : m_str_WCHAR(NULL),
                           m_str_wchar(NULL)
{
    if (str)
    {
        int len = getLenShortWcharStr(str);
        m_str_WCHAR = new WCHAR_T[len + 1];
        memset(m_str_WCHAR,   0, sizeof(WCHAR_T) * (len + 1));
        memcpy(m_str_WCHAR, str, sizeof(WCHAR_T) * len);
        ::convFromShortWchar(&m_str_wchar, m_str_WCHAR);
    }
}
#endif
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
        delete [] m_str_WCHAR;
        m_str_WCHAR = NULL;
    }
#endif
    if (m_str_wchar)
    {
        delete [] m_str_wchar;
        m_str_wchar = NULL;
    }
}
//---------------------------------------------------------------------------//

