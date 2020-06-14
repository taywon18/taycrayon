#include "gtakeybinder.h"
#include <QChar>
#include <QString>
#include <QVector>
#include <QDebug>
#include <QRegularExpression>
#include "keymap.h"
#include <algorithm>
#include "utils.h"
#include "gtainformation.h"
#include <QGuiApplication>
#include <QClipboard>
#include <cmath>
#include <algorithm>
#include <QVector2D>

GtaKeybinder* GtaKeybinder::_instance = nullptr;

GtaKeybinder::GtaKeybinder(QObject *parent)
    : QObject(parent)
{
    _instance = this;
    _finder.initFromBase();
    inject();
}

void GtaKeybinder::keybind(Bind b)
{
    HANDLE gtaProc = getOpenedGtaProcess();

    QList<BindPossibility> validpossibilities;
    BindEnvironment be = getBindEnvironment(gtaProc);
    foreach(BindPossibility bp, b.possibilities())
    {
        BindRequierments br = bp.requierments();

        if (br.match(be))
            validpossibilities.append(bp);
    }

    QList<BindPossibility> bps = validpossibilities;
    std::random_shuffle(bps.begin(), bps.end());

    keybind(bps, gtaProc);

    CloseHandle(gtaProc);
}

void GtaKeybinder::directKeybind(const QString &str)
{
    directKeybind(str, SpecialBindOptions());
}

void GtaKeybinder::keybind(QList<BindPossibility> bps, HANDLE proc)
{
    foreach(BindPossibility bp, bps)
    {
        bool force = !bp.doNotSendIfParameterMissing();
        if(keybind(bp, proc) || force)
            return;
    }
}

bool GtaKeybinder::keybind(BindPossibility bp, HANDLE gtaProcess)
{
    bool force = !bp.doNotSendIfParameterMissing();
    QString str = bp.content();

    QRegularExpression regexp("\\${{([. a-zA-Z]+)}}");

    QSet<QString> lst;
    QRegularExpressionMatchIterator i = regexp.globalMatch(str);
    while (i.hasNext())
    {
        QRegularExpressionMatch match = i.next();

        auto caps = match.capturedTexts();
        lst.insert(caps[1]);
    }

    bool hasmissing;
    QList<QPair<QString, QString> > replacment = replace(lst, gtaProcess, &hasmissing);
    if(!force && hasmissing)
        return false;

    foreach(auto r, replacment)
        str.replace("${{" + r.first + "}}", r.second);

    directKeybind(str, SpecialBindOptions(bp));
    return true;
}

GtaKeybinder::~GtaKeybinder()
{
    if(_hook)
        UnhookWindowsHookEx(_hook);

    _instance = nullptr;
}


GtaKeybinder &GtaKeybinder::instance()
{
    if(_instance == nullptr)
        _instance = new GtaKeybinder();

    return *_instance;
}


LRESULT CALLBACK LowLevelKeyboardProc( int nCode,
                                       WPARAM wParam,
                                       LPARAM lParam)
{

    (GtaKeybinder::instance().onKey(nCode, wParam, lParam)) ? 1 : CallNextHookEx( NULL, nCode, wParam, lParam );
}

void GtaKeybinder::inject()
{
    _hook  = SetWindowsHookEx( WH_KEYBOARD_LL,
    LowLevelKeyboardProc,
    NULL,
    0 );
    if(!_hook)
        qDebug() << "Echec de la mise en route une fois !";
}

INPUT InputZero()
{
    INPUT ret;
    ZeroMemory(&ret, sizeof(INPUT));
    return ret;
}

INPUT InputKey(DWORD key, bool release = false)
{
    INPUT ret = InputZero();
    ret.type = 1;
    ret.ki.wVk = LOBYTE(key);
    ret.ki.wScan = MapVirtualKey(LOBYTE(key), 0);
    if(release)
        ret.ki.dwFlags = KEYEVENTF_KEYUP;

    return ret;
}

void GtaKeybinder::directKeybind(const QString& str, SpecialBindOptions sbo)
{
    if(!_active)
        return;

    _active = false;
    QVector<INPUT> inputs;

    if(sbo.RemoveCapsLock && (GetKeyState(VK_CAPITAL) & 0x0001) != 0)
    {
        inputs.append(InputKey(VK_CAPITAL, false));
        inputs.append(InputKey(VK_CAPITAL, true));
    }

    bool majPressed = (GetKeyState( VK_SHIFT ) & 0x8000);
    if(majPressed)
        inputs.append(InputKey(VK_SHIFT, true));

    bool ctrlPressed = (GetKeyState( VK_CONTROL ) & 0x8000);
    if(ctrlPressed)
        inputs.append(InputKey(VK_CONTROL, true));

    bool altPressed = (GetKeyState( VK_MENU ) & 0x8000);
    if(altPressed)
        inputs.append(InputKey(VK_MENU, true));

	auto lines = str.split("\n", QString::SkipEmptyParts);
	qDebug() << "Keybinding lines: " << lines.length();
	foreach(auto line, lines)
    {
		qDebug() << "Keybinding new line";
		if(sbo.OpenChatEachBeginLine && !_chatOpen)
        {
			inputs.append(InputKey(0x54, false));
            inputs.append(InputKey(0x54, true));
			_chatOpen = true;
			qDebug() << "opening chat";
        }

        if(sbo.CleanChatBeforeBind)
        {
            // CTRL + a
            inputs.append(InputKey(VK_CONTROL, false));
            inputs.append(InputKey(0x51, false));
            inputs.append(InputKey(0x51, true));
            inputs.append(InputKey(VK_CONTROL, true));

            // suppr
            inputs.append(InputKey(VK_DELETE, false));
            inputs.append(InputKey(VK_DELETE, true));
        }

        if(sbo.PutCursorAtBeginBeforeBind)
        {
            inputs.append(InputKey(VK_HOME, false));
            inputs.append(InputKey(VK_HOME, true));
        }

        int pos = 0;
		while (pos < line.length())
        {
			QChar c = line.at(pos);
            if(c == '~')
            {
                pos++;

                inputs.append(InputKey(VK_RETURN));
                inputs.append(InputKey(VK_RETURN, true));
				_chatOpen = false;
                continue;
			}

			if(c == 't')
			{
				_chatOpen = true;
			}

			wchar_t cChar = c.unicode();
			qDebug() << c << cChar;

			short nKeyScan = VkKeyScanW(cChar);

            bool bShift = (HIBYTE(nKeyScan) & 1?1:0);
            bool bControl = (HIBYTE(nKeyScan) & 2?1:0);
            bool bAlt = (HIBYTE(nKeyScan) & 4?1:0);

            if (bShift)
                inputs.append(InputKey(VK_SHIFT));
            if (bControl)
                inputs.append(InputKey(VK_CONTROL));
            if (bAlt)
                inputs.append(InputKey(VK_MENU));

            inputs.append(InputKey(LOBYTE(nKeyScan)));
            inputs.append(InputKey(LOBYTE(nKeyScan), true));

            if (bShift)
                inputs.append(InputKey(VK_SHIFT, true));
            if (bControl)
                inputs.append(InputKey(VK_CONTROL, true));
            if (bAlt)
                inputs.append(InputKey(VK_MENU, true));


            pos++;
        }

        if(sbo.PutCursorAtEndAfterBind)
        {
            inputs.append(InputKey(VK_END, false));
            inputs.append(InputKey(VK_END, true));
        }

        if(sbo.SendChatEachEndLine)
        {
            inputs.append(InputKey(VK_RETURN, false));
            inputs.append(InputKey(VK_RETURN, true));
			_chatOpen = false;
        }
    }



    SendInput(inputs.size(),inputs.data(), sizeof(INPUT));
    _active = true;
}
template<typename T>
T angleDiff(T a, T b)
{
    a += M_PI;
    b += M_PI;

    T phi = std::fmod<T>(std::abs(b - a), M_PI * 2);       // This is either the distance or 360 - distance
    T distance = phi > M_PI ? M_PI * 2 - phi : phi;
    return distance;
}

QList<QPair<QString, QString> > GtaKeybinder::replace(QSet<QString> keywords, HANDLE h, bool* missing)
{
    QList<QPair<QString, QString> > replacments;

    if(keywords.empty())
        return replacments;

    bool hasCreatedh = false;
    if(!h){
        hasCreatedh = true;
        h = getOpenedGtaProcess();
    }

    if(!h)
        return replacments;

    DWORD address = 0xB6F5F0;
    unsigned int Player;
    ReadProcessMemory(h, (LPCVOID)address, &Player, sizeof(Player), 0);

    address = Player + 0x14;
    unsigned int PlayerMatrix;
    ReadProcessMemory(h, (LPCVOID)address, &PlayerMatrix, sizeof(PlayerMatrix), 0);

    float PlayerPosX, PlayerPosY, PlayerPosZ, PlayerRotZ;
    ReadProcessMemory(h, (void*)(PlayerMatrix + 0x30), (void*)&PlayerPosX, sizeof(float), NULL);
    ReadProcessMemory(h, (void*)(PlayerMatrix + 0x34), (void*)&PlayerPosY, sizeof(float), NULL);
    ReadProcessMemory(h, (void*)(PlayerMatrix + 0x38), (void*)&PlayerPosZ, sizeof(float), NULL);
    ReadProcessMemory(h, (void*)(Player + 0x558), (void*)&PlayerRotZ, sizeof(float), NULL);



    address = Player + 0x530;
    DWORD DrivingState;
    ReadProcessMemory(h, (LPCVOID)address, &DrivingState, sizeof(DrivingState), 0);
    bool isDriving = (DrivingState == 50);

    unsigned int CurrentVehiclePtr = 0;
    if(isDriving)
    {
        address = Player + 0x58C;
        ReadProcessMemory(h, (LPCVOID)address, &CurrentVehiclePtr, sizeof(CurrentVehiclePtr), 0);

        //TODO: find position
    }


    address = 0xB74494;
    unsigned int VehiclePoolInfoPtr;
    ReadProcessMemory(h, (LPCVOID)address, &VehiclePoolInfoPtr, sizeof(VehiclePoolInfoPtr), 0);

    address = VehiclePoolInfoPtr + 8;
    DWORD VehicleCountInPool;
    ReadProcessMemory(h, (LPCVOID)address, &VehicleCountInPool, sizeof(VehicleCountInPool), 0);

    address = VehiclePoolInfoPtr + 0;
    unsigned int VehicleFirstVehPtr;
    ReadProcessMemory(h, (LPCVOID)address, &VehicleFirstVehPtr, sizeof(VehicleFirstVehPtr), 0);

    unsigned int PoolStart = VehicleFirstVehPtr;

    unsigned int VehTargetPtr = 0;
    float minDistanceSq = std::numeric_limits<float>::max();
    float VehTargetX, VehTargetY, VehTargetZ, VehTargetDeltaRot;
    unsigned int VehTargetDriver;
    unsigned char VehTargetPassengerCount;
    WORD VehTargetModel;

    auto plyRotZCorrected = PlayerRotZ + M_PI + M_PI;
    plyRotZCorrected = std::fmod(plyRotZCorrected, 2 * M_PI);

    for(int vi = 0; vi < VehicleCountInPool; vi++)
    {
        unsigned int VehPtr = PoolStart + 0xA18 * vi;

        if(VehPtr == CurrentVehiclePtr)
            continue;

        WORD vehmodel;
        ReadProcessMemory(h, (void*)(VehPtr + 34), (void*)&vehmodel, sizeof(vehmodel), NULL);
        if(vehmodel == 0)
            continue;

        address = VehPtr + 20;
        unsigned int VehicleMatrixPtr;
        ReadProcessMemory(h, (LPCVOID)address, &VehicleMatrixPtr, sizeof(VehicleMatrixPtr), 0);

        float vehx, vehy, vehz;
        ReadProcessMemory(h, (void*)(VehicleMatrixPtr + 48), (void*)&vehx, sizeof(vehx), NULL);
        ReadProcessMemory(h, (void*)(VehicleMatrixPtr + 52), (void*)&vehy, sizeof(vehy), NULL);
        ReadProcessMemory(h, (void*)(VehicleMatrixPtr + 56), (void*)&vehz, sizeof(vehz), NULL);

        unsigned int VehicleDriver;
        ReadProcessMemory(h, (void*)(VehPtr + 1120), (void*)&VehicleDriver, sizeof(VehicleDriver), NULL);

        unsigned char vehpassengerCount = 0;
        unsigned int VehiclePassenger1, VehiclePassenger2, VehiclePassenger3;
        ReadProcessMemory(h, (void*)(VehPtr + 1124), (void*)&VehiclePassenger1, sizeof(VehiclePassenger1), NULL);
        ReadProcessMemory(h, (void*)(VehPtr + 1128), (void*)&VehiclePassenger2, sizeof(VehiclePassenger2), NULL);
        ReadProcessMemory(h, (void*)(VehPtr + 1132), (void*)&VehiclePassenger3  , sizeof(VehiclePassenger3), NULL);

        if(VehicleDriver)
            vehpassengerCount++;
        if(VehiclePassenger1)
            vehpassengerCount++;
        if(VehiclePassenger2)
            vehpassengerCount++;
        if(VehiclePassenger3)
            vehpassengerCount++;

        if(!VehicleDriver)
            continue;

        QVector2D plytoveh3(vehx - PlayerPosX, vehy - PlayerPosY);
        auto plytovehU3 = plytoveh3.normalized();
        float plytovehAngle3 = std::atan2(- plytovehU3.y(), - plytovehU3.x());
        plytovehAngle3 = plytovehAngle3 + M_PI + (M_PI * 3/2);
        plytovehAngle3 = std::fmod(plytovehAngle3, 2 * M_PI);
        float da = angleDiff<float>(plyRotZCorrected, plytovehAngle3);

        if(da > M_PI / 6)
            continue;

        // <!> Only accurate between PI/2 and -PI/2

        float newDistanceSq = distanceSq(PlayerPosX, PlayerPosY, PlayerPosZ, vehx, vehy, vehz);
        if(newDistanceSq < minDistanceSq)
        {
            minDistanceSq = newDistanceSq;
            VehTargetX = vehx;
            VehTargetY = vehy;
            VehTargetZ = vehz;
            VehTargetPtr = VehPtr;
            VehTargetModel = vehmodel;
            VehTargetDeltaRot = da;
            VehTargetDriver = VehicleDriver;
            VehTargetPassengerCount = vehpassengerCount;
        }
    }

    float maxAcceptableDistance = 50;
    bool hasTarget = (minDistanceSq < maxAcceptableDistance * maxAcceptableDistance);

    if(missing)
        *missing = false;

    bool hasMissing = false;
    foreach(QString k, keywords)
    {
        if(k.toLower() == "perso.cash")
        {
            DWORD i;
            ReadProcessMemory(h, (void*)0xB7CE50, (void*)&i, sizeof(i), NULL);
            replacments.append(qMakePair(k, QString::number(i)));;
        }
        else if(k.toLower() == "quartier")
        {
            replacments.append(qMakePair(k, _finder.find(PlayerPosX, PlayerPosY)));
        }
        else if(k.toLower() == "coller" || k.toLower() == "paste")
        {
            QClipboard *clipboard = QGuiApplication::clipboard();

            replacments.append(qMakePair(k, clipboard->text()));
        }
        else if(k.toLower() == "direction")
        {
            replacments.append(qMakePair(k, _finder.gtaAngleToDirection(PlayerRotZ)));
        }
        else if(k.toLower() == "cible.modele")
        {
            if(hasTarget)
                replacments.append(qMakePair(k, GtaInformation::Instance().getName(VehTargetModel) ));
            else
                hasMissing = true;
        }
        else if(k.toLower() == "cible.npassage")
        {
            if(hasTarget)
                replacments.append(qMakePair(k, QString::number(VehTargetPassengerCount) ));
            else
                hasMissing = true;
        }
        else
            hasMissing = true;

    }

    if(hasMissing && missing)
        *missing = true;

    if(hasCreatedh)
        CloseHandle(h);

    return replacments;
}

bool GtaKeybinder::onKey(int nCode, WPARAM wParam, LPARAM lParam)
{
    if(!_binds)
        return false; // do not block

    if(!_active)
        return false;

    if(!GetSampForegroundWindows())
        return false;

    if (nCode >= 0)
    {
            // the action is valid: HC_ACTION.
            if (wParam == WM_KEYUP)
            {
                // lParam is the pointer to the struct containing the data needed, so cast and assign it to kdbStruct.
                KBDLLHOOKSTRUCT kbdStruct = *((KBDLLHOOKSTRUCT*)lParam);

                if(kbdStruct.vkCode == 0x54){
                    _chatOpen = true;
                }
                else if(kbdStruct.vkCode == VK_RETURN || kbdStruct.vkCode == VK_ESCAPE){
                    _chatOpen = false;
                }

                bool majPressed = (GetKeyState( VK_SHIFT ) & 0x8000);
                bool ctrlPressed = (GetKeyState( VK_CONTROL ) & 0x8000);
                bool altPressed = (GetKeyState( VK_MENU ) & 0x8000);

                for(Bind b : *_binds)
                {
                    if(b.keySequence().count() == 0)
                        continue;

                    if(!match(b.keySequence(), kbdStruct.vkCode, ctrlPressed, majPressed, altPressed))
                        continue;

                    qDebug() << "detected " << b.keySequence();

                    keybind(b);
                }
        }
    }
    return false; // do not block
}

bool GtaKeybinder::match(QKeySequence seq, int nCode, bool ctrlpressed, bool shiftpressed, bool altpressed)
{
    if(!seq.count())
        return false;

    auto map = KeyMapping::keyMap();

    auto keyFound = map.find(nCode);
    int keyCode = (keyFound != map.end() ? keyFound.value() : -1);

    if(!keyCode)
        return false;

    int modifiers = 0;
    if(ctrlpressed) modifiers += Qt::ControlModifier;
    if(shiftpressed) modifiers += Qt::ShiftModifier;
    if(altpressed) modifiers += Qt::AltModifier;

    int keycodeWithModifier = keyCode + modifiers;

    return seq[0] == keycodeWithModifier;
}

HANDLE GtaKeybinder::getOpenedGtaProcess(bool foregroundOnly)
{
    HANDLE ret = 0;

	HWND gtahwnd = foregroundOnly ? GetSampForegroundWindows() : GetSampWindows();
    DWORD gtapid = 0;
    DWORD gtathread = GetWindowThreadProcessId( gtahwnd, &gtapid );
    if(!gtapid)
        return ret;

    return OpenProcess(PROCESS_VM_READ, false, gtapid);
}



BindEnvironment GtaKeybinder::getBindEnvironment(HANDLE h)
{
    BindEnvironment ret;
    ret.chatOpen = _chatOpen;

    DWORD address = 0xB6F5F0;
    int Player;
    ReadProcessMemory(h, (LPCVOID)address, &Player, sizeof(Player), 0);

    address = Player + 0x540;
    float Heatlh;
    ReadProcessMemory(h, (LPCVOID)address, &Heatlh, sizeof(Heatlh), 0);
    ret.live = Heatlh;

    address = Player + 0x548;
    float Armor;
    ReadProcessMemory(h, (LPCVOID)address, &Armor, sizeof(Armor), 0);
    ret.armor = Armor;

    address = Player + 0x530;
    DWORD DrivingState;
    ReadProcessMemory(h, (LPCVOID)address, &DrivingState, sizeof(DrivingState), 0);

	address = Player + 0x14;
	unsigned int PlayerMatrix;
	ReadProcessMemory(h, (LPCVOID)address, &PlayerMatrix, sizeof(PlayerMatrix), 0);

	float PlayerPosX, PlayerPosY, PlayerPosZ, PlayerRotZ;
	ReadProcessMemory(h, (void*)(PlayerMatrix + 0x30), (void*)&PlayerPosX, sizeof(float), NULL);
	ReadProcessMemory(h, (void*)(PlayerMatrix + 0x34), (void*)&PlayerPosY, sizeof(float), NULL);
	ReadProcessMemory(h, (void*)(PlayerMatrix + 0x38), (void*)&PlayerPosZ, sizeof(float), NULL);
	ReadProcessMemory(h, (void*)(Player + 0x558), (void*)&PlayerRotZ, sizeof(float), NULL);

	ret.posX = PlayerPosX;
	ret.posY = PlayerPosY;
	ret.posZ = PlayerPosZ;

	if(DrivingState != 50)
        return ret;

    address = Player + 0x58C;
    DWORD CarPtr;
    ReadProcessMemory(h, (LPCVOID)address, &CarPtr, sizeof(CarPtr), 0);

    address = CarPtr + 1216;
    float CarLife;
    ReadProcessMemory(h, (LPCVOID)address, &CarLife, sizeof(CarLife), 0);
    ret.vehLive = CarLife;

    address = CarPtr + +34;
    WORD CarType;
    ReadProcessMemory(h, (LPCVOID)address, &CarType, sizeof(CarType), 0);



    return ret;
}

HWND GtaKeybinder::GetSampWindows()
{
	HWND windobe = FindWindow(
				NULL,
				L"GTA:SA:MP"
			  );
	if(windobe)
			return windobe;
	else
			return nullptr;
}

HWND GtaKeybinder::GetSampForegroundWindows()
{
    HWND windobe = GetForegroundWindow();
    WCHAR name[256];
     GetWindowText(windobe,name,255);
    if(wcscmp(name, L"GTA:SA:MP") == 0)
            return windobe;
    else
            return nullptr;
}

void GtaKeybinder::setBinds(BindContainer *binds)
{
    _binds = binds;
}

template<typename F>
F GtaKeybinder::distance(F ax,F ay, F az, F bx, F by, F bz)
{
    F sqd =distanceSq(ax, ay, az, bx, by, bz);
    return std::sqrt(sqd);
}

template<typename F>
F GtaKeybinder::distanceSq(F ax, F ay, F az,F bx, F by, F bz)
{
    return std::pow(bx - ax, 2) + std::pow(by - ay, 2) + std::pow(bz - az, 2);
}

GtaKeybinder::SpecialBindOptions::SpecialBindOptions(BindPossibility &bp)
{
    CleanChatBeforeBind = bp.cleanChatBeforeBind();
    OpenChatEachBeginLine = bp.openChatEachBeginLine();
    SendChatEachEndLine = bp.sendChatEachEndLine();
    RemoveCapsLock = bp.removeCapsLock();
    PutCursorAtBeginBeforeBind = bp.putCursorAtFirstPositionWhenStart();
    PutCursorAtEndAfterBind = bp.putCursorAtLastPositionWhenEnd();

	qDebug() << "created bp:" << CleanChatBeforeBind << OpenChatEachBeginLine << SendChatEachEndLine << RemoveCapsLock << PutCursorAtBeginBeforeBind << PutCursorAtEndAfterBind;
}
