/*
# Программа INTERPOLATION
# Курсовой проект по предмету МДК.03.01 Технология разработки программного обеспечения
# Разработал: Смыслов Александр Андреевич
# Дата: 03.03.2020
# Программа написана на языке C++
# Задание:
#       Разработать программу интерполяции функций с использованием:
#        - интерполяционного многочлена Лагранжа;
#       - интерполяционных многочленов Ньютона;
# Используемые переменные в основной программе:
#   X - вектор аргументов функции;
#   Y - вектор значений функции;
#   cnt - количество координат;
#   pX - интерполируемая точка;
#   pY - результат интерполяции;
#   method - флаг выбранного метода.
# Используемые функции:
#   StrToInt() - преобразование строки в целое число;
#   StrToFlo() - преобразование строки в вещественное число;
#   checkNum() - проверка строки на наличие поторонних символов, отличных от цифр;
#   IntToStr() - преобразование целого числа в строку;
#   FloToStr() - преобразование вещественного числа в строку;
#   showXY() - отрисовка полей ввода данных;
#   Ln() - интерполирование полиномом Лагранжа;
#   fact() - подсчет факториала;
#   Pn() - интерполирование полиномом Ньютона;
#   WndProc() - поток обработки меню программы;
#   WndData() - поток обработки окна ввода данных;
#   WndGraph() - поток обработки окна черчения графика.
*/




#include <windows.h>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;


HINSTANCE hInst;
LRESULT WINAPI WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT WINAPI WndGraph(HWND, UINT, WPARAM, LPARAM);
LRESULT WINAPI WndData(HWND, UINT, WPARAM, LPARAM);
HWND hwndLag, hwndNew, hwndGraph;
WNDCLASS w1, wG, wLag, wNew;
int cnt = 0;
bool method = false;
static vector<HWND> hX;
static vector<HWND> hY;
vector<double>X;
vector<double>Y;
TCHAR val[20];
int pX = 0;

// Инициализация окон
/*
    Формальные параметры:
    hInstance - дескриптор текущего экземпляра приложения;
    hPrevInstance - дескриптор предыдущего экземпляра приложения;
    lpCmdLine - командная строка приложения;
    nCmdShow - определение окна.
    Локальные параметры:
    hwnd - дескриптор главного окна;
    msg - структура сообщений.
*/
int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    HWND hwnd; 
    MSG msg; 

    // Инициализация всех окон программы
    memset(&w1, 0, sizeof(WNDCLASS));
    memset(&wG, 0, sizeof(WNDCLASS));
    memset(&wLag, 0, sizeof(WNDCLASS));
    memset(&wNew, 0, sizeof(WNDCLASS));
    w1.style = CS_HREDRAW | CS_VREDRAW;
    w1.lpfnWndProc = WndProc;
    w1.hInstance = hInstance;
    w1.hbrBackground = (HBRUSH)(WHITE_PEN);
    w1.lpszClassName = "My Class";
    RegisterClass(&w1);
    wG.style = CS_HREDRAW | CS_VREDRAW;
    wG.lpfnWndProc = WndGraph;
    wG.hInstance = hInstance;
    wG.hbrBackground = (HBRUSH)(WHITE_PEN);
    wG.lpszClassName = "My Class2";
    RegisterClass(&wG);
    wLag.style = CS_HREDRAW | CS_VREDRAW;
    wLag.lpfnWndProc = WndData;
    wLag.hInstance = hInstance;
    wLag.hbrBackground = (HBRUSH)(WHITE_PEN);
    wLag.lpszClassName = "My Class3";
    RegisterClass(&wLag);
    wNew.style = CS_HREDRAW | CS_VREDRAW;
    wNew.lpfnWndProc = WndData;
    wNew.hInstance = hInstance;
    wNew.hbrBackground = (HBRUSH)(WHITE_PEN);
    wNew.lpszClassName = "My Class4";
    RegisterClass(&wNew);

    // Создание главного окна
    hwnd = CreateWindow("My Class", "Курсовой проект",
        WS_OVERLAPPED | WS_SYSMENU | WS_MINIMIZEBOX, 100, 150, 400, 300, nullptr, nullptr, hInstance, nullptr);
    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);          

    // Цикл обработки сообщений
    hInst = hInstance;
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return (int)msg.wParam;
}


// Преобразование строки в целое число
/*
    Формальные параметры:
    s - строка, подлежащая преобразованию.
    Локальные параметры:
    temp - результат преобразования;
    i - индекс преобразовываемого символа;
    sign - флаг наличия знака минус перед числом.
*/
int StrToInt(char* s)
{
    int temp = 0;
    int i = 0;
    int sign = 0;

    // Если обнаружен минус, поставить флаг
    if (s[i] == '-')
    {
        sign = 1;
        i++;
    }

    //Преобразование строки в число
    while (s[i] >= 0x30 && s[i] <= 0x39)
    {
        temp = temp + (s[i] & 0x0F);
        temp = temp * 10;
        i++;
    }
    temp = temp / 10;

    // Если флаг равен единице, значит число отрицательное
    if (sign == 1)
        temp = -temp;
    return(temp);
}

// Преобразование строки в вещественное число
/*
    Формальные параметры:
    s - строка, подлежащая преобразованию.
    Локальные параметры:
    temp - результат преобразования;
    demp - результат дробного преобразования
    i - индекс преобразовываемого символа;
    sign - флаг наличия знака минус перед числом.
*/
float StrToFlo (char* s)
{
    float temp = 0;
    float demp = 0;
    int i = 0;
    int sign = 0;

    // Если обнаружен минус, поставить флаг
    if (s[i] == '-')
    {
        sign = 1;
        i++;
    }

    //Преобразование строки в число
    while (s[i] >= 0x30 && s[i] <= 0x39)
    {
        temp = temp + (s[i] & 0x0F);
        temp = temp * 10;
        i++;
    }

    // Если обнаружена точка или запятая,
    // преобразовать оставшуюся часть строки как дробную
    if ((s[i] == '.') || (s[i] == ',')) {
        i++;
        while (s[i] >= 0x30 && s[i] <= 0x39)
        {
            demp = demp + (s[i] & 0x0F);
            demp = demp * 10;
            i++;
        }
        while (demp >= 1)
        {
            demp = demp / 10;
        }
    }

    temp = temp / 10;
    temp = temp + demp;

    // Если флаг равен единице, значит число отрицательное
    if (sign == 1)
        temp = -temp;
    
    return(temp);
}

// Проверка строки на наличие лишних символов
/*
    Формальные параметры:
    s - строка, подлежащая проверке;
    f - флаг типа проверки (целого или вещественного числа).
    Локальные параметры:
    i - индекс проверяемого символа.
*/
bool checkStr(char* s, bool f) {
    int i = 0;
    if (s[i] == '-')
        i++;
    while (s[i] >= 0x30 && s[i] <= 0x39)
    {
        i++;
    }
    if (f)
        if ((s[i] == '.')|| (s[i] == ','))
            i++;
    while (s[i] >= 0x30 && s[i] <= 0x39)
    {
        i++;
    }
    if (s[i] != '\0')
        return false;
    return(true);
}


// Преобразование целого числа в строку
/*
    Формальные параметры:
    n - преобразовываемое число.
    Локальные параметры:
    s - результат преобразования в перевернутом виде;
    t - преобразованный символ;
    temp - результат преобразования;
    i - счетчик символов строки s;
    k - счетчик символов строки temp;
    sign - флаг знака перед числом.
*/
char* IntToStr(int n)
{
    char s[40], t, * temp;
    int i, k;
    int sign = 0;
    i = 0;
    k = n;

    // Если число меньше нуля, поставить флаг
    if (k < 0)
    {
        sign = 1;
        k = -k;
    }

    // Поразрядное преобразование
    do {
        t = k % 10;
        k = k / 10;
        s[i] = t | 0x30;
        i++;
    } while (k > 0);

    if (sign == 1)
    {
        s[i] = '-';
        i++;
    }
    temp = new char[i];
    k = 0;
    i--;
    // Сохранение результата в привычном виде
    while (i >= 0) {
        temp[k] = s[i];
        i--; k++;
    }
    temp[k] = '\0';
    return(temp);
}

// Преобразование вещественного числа в строку
/*
    Формальные параметры:
    n - преобразовываемое число.
    Локальные параметры:
    s - результат преобразования в перевернутом виде;
    d - результат дробного преобразования в перевернутом виде;
    t - преобразованный символ;
    temp - результат преобразования;
    dr - дробная часть числа;
    i - счетчик символов строки s;
    i2 - счетчик символов строки d;
    k - счетчик символов строки temp;
    k2 - дополнительный счетчик символов строки temp;
    sign - флаг знака перед числом.
*/
char* FloToStr(float n)
{
    char s[40], d[40], t, * temp;
    float dr;
    int i, k, k2, i2;
    int sign = 0;
    i = 0;
    i2 = 0;

    k = int(n); // Сохраняется целая часть числа
    dr = n - k; // Сохраняется дробная часть числа

    // Флаг знака перед числом
    if (n < 0)
    {
        sign = 1;
        k = abs(k);
    }

    // Целочисленное преобразование
    do {
        t = k % 10;
        k = k / 10;
        s[i] = t | 0x30;
        i++;
    } while (k > 0);
    

    // Дробное преобразование
    dr *= 100;
    k2 = int(abs(dr));
    do {
        t = k2 % 10;
        k2 = k2 / 10;
        d[i2] = t | 0x30;
        i2++;
    } while (k2 > 0);
    if (sign == 1)
    {
        s[i] = '-';
        i++;
    }
    if (i2 == 0) {
        temp = new char[i];
    }
    else
    {
        temp = new char[i + i2 + 1];
        i2--;
    }
    k = 0;
    i--;

    while (i >= 0) {
        temp[k] = s[i];
        i--; k++;
    }
    if (i2 > 0) {
        temp[k] = '.';
        k++;
        while (i2 >= 0) {
            temp[k] = d[i2];
            i2--; k++;
        }
    }
    temp[k] = '\0';
    return(temp);
}

// Отрисовка полей ввода данных
/*
    Локальные параметры:
    i - индекс поля для ввода данных.
*/
void showXY() {
    for (int i = 0; i < cnt; i++) {

        ShowWindow(hX[i], SW_SHOWNORMAL);
        ShowWindow(hY[i], SW_SHOWNORMAL);
    }
}

// Интерполирование методом Лагрнажа
/*
    Формальные параметры:
    X - вектор точек по оси X;
    Y - вектор точек по оси Y;
    t - интерполируемое значение.
    Локальные параметры:
    sum - результат интерполривания;
    num - формируемый числитель нового слагаемого;
    denom - формируемый знаменатель нового слагаемого;
    n - размер вектора.
*/
double Ln(vector<double>X, vector<double>Y, double t) {
    double sum, num, denom;
    sum = 0;
    int n = X.size();
    for (int j = 0; j < n; j++) {
        num = 1; denom = 1;
        for (int i = 0; i < n; i++) {
            if (i == j) {
                num = num * 1; denom = denom * 1;
            }
            else {
                num = num * (t - X[i]);
                denom = denom * (X[j] - X[i]);
            }
        }
        sum = sum + Y[j] * num / denom;
    }
    return sum;
}

// Подсчет факториала
/*
    Формальные параметры:
    N - число, факториал которого нужно найти.
    Локальные параметры:
    f - факториал.
*/
long double fact(int N)
{
    double f = 1;
    for (int i = 2; i <= N; i++) {
        f = f * i;
    }
    return f;
}

// Интерполирование методом Ньютона
/*
    Формальные параметры:
    X - вектор точек по оси X;
    Y - вектор точек по оси Y;
    t - интерполируемое значение.
    Локальные параметры:
    deltaY - вектор разностей между Y;
    result - результат интерполирования;
    num - формируемый числитель нового слагаемого;
    denom - формируемый знаменатель нового слагаемого;
    defX  - новый множитель результата;
    n - размер вектора;
    cntD - количетсво разностей.
*/
double Pn(vector<double>X, vector<double>Y, double t) {
    vector<double> deltaY;
    double result = Y[0], num = 0, denom = 0, defX = (t - X[0]) / (X[1] - X[0]), MulDefX = 0;
    int n = Y.size(), cntD = n;
    do {
        for (int i = 0; i < n; i++) {
            X[i] = Y[i];
        }
        for (int i = 0; i < (n - 1); i++) {
            Y[i] = X[i + 1] - X[i];
        }
        deltaY.push_back(Y[0]);
        cntD--;
    } while (cntD != 1);
    result += deltaY[0] * defX;
    num = defX;
    for (int i = 1; i < deltaY.size(); i++) {
        num = num * (defX - i);
        denom = fact(i + 1);
        result = result + ((num / denom) * deltaY[i]);
    }
    return result;
}

// Поток обработки главного меню
/*
    Формальные параметры:
    hWnd - дескриптор родительского окна;
    message - сообщения;
    wParam - дополнительная информация о сообщениях;
    lParam - дополнительная информация о сообщениях.
    Локальные параметры:
    hEdt1 - поле для ввода количества точек;
    hPlus - кнопка, увличивающая счетчик интерполируемых узлов на 1;
    hMinus - кнопка, уменьшающая счетчик интерполируемых узлов на 1;
    hLag - кнопка выбора интерполирования методом Лагранжа;
    hNewton - кнопка выбора интерполирования методом Ньютона;
    ps - структра рисования клиентской области окна;
    hdc - дескриптор контекста устройства.
*/
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    static HWND hEdt1;
    static HWND hPlus, hMinus,  hLag, hNewton;
    PAINTSTRUCT ps; 
    HDC hdc;
    switch (message)
    {
    case WM_CREATE: // Инициализация объектов окна
        
        hEdt1 = CreateWindow("edit", IntToStr(cnt),
            WS_CHILD | WS_VISIBLE | WS_BORDER | ES_RIGHT , 40, 50, 60, 20,
            hWnd, 0, hInst, NULL);
        hPlus = CreateWindow("button", "+",
            WS_CHILD | WS_VISIBLE | WS_BORDER ,
            140, 50, 30, 30, hWnd, 0, hInst, NULL);
        hMinus = CreateWindow("button", "-",
            WS_CHILD | WS_VISIBLE | WS_BORDER,
            180, 50, 30, 30, hWnd, 0, hInst, NULL);
        hLag = CreateWindow("button", "Лагранж",
            WS_CHILD | WS_VISIBLE | WS_BORDER,
            50, 100, 100, 30, hWnd, 0, hInst, NULL);
        hNewton = CreateWindow("button", "Ньютон",
            WS_CHILD | WS_VISIBLE | WS_BORDER,
            50, 150, 100, 30, hWnd, 0, hInst, NULL);
        break;
    case WM_COMMAND: // Выполнение команд по нажатию на кнопки
    {
        
        if ((lParam == (LPARAM)hNewton) || ((lParam == (LPARAM)hLag))) {
            if ((IsWindowVisible(hwndLag)) || (IsWindowVisible(hwndNew))) {
                MessageBox(hWnd, "Закройте предыдущее окно!", "Ошибка!", MB_ICONERROR);
            }
            else {
                if ((lParam == (LPARAM)hNewton)) { // Вызов окна интерполирования методом Ньютона

                    method = false;
                    GetWindowText(hEdt1, val, 20);
                    if (checkStr(val, 0)) {
                        cnt = StrToInt(val);
                        if ((cnt >= 2)&&(cnt<=10)) {
                            hwndNew = CreateWindow("My Class4", "Вввод данных для метода Ньютона",
                                WS_OVERLAPPED | WS_SYSMENU | WS_MINIMIZEBOX, 500, 150, 400, 260 + ((cnt - 2) * 40), nullptr, nullptr, hInst, nullptr);
                            ShowWindow(hwndNew, 1);
                            UpdateWindow(hwndNew);
                        }
                        else if (cnt < 0) {
                            cnt = 0;
                            SetWindowText(hEdt1, IntToStr(cnt));
                            MessageBox(hWnd, "Количество не может быть отрицательным", "Ошибка!", MB_ICONERROR);
                        }
                        else if (cnt > 10){
                        
                            cnt = 0;
                            SetWindowText(hEdt1, IntToStr(cnt));
                            MessageBox(hWnd, "Ограничение точек неболее 10", "Ошибка!", MB_ICONERROR);
                        }

                        else
                            MessageBox(hWnd, "Для интерполирования\nнужно минимум две точки", "Ошибка!", MB_ICONERROR);
                    }
                    else
                        MessageBox(hWnd, "Требуется ввести целое число", "Ошибка!", MB_ICONERROR);
                }
                if ((lParam == (LPARAM)hLag)) {// Вызов окна интерполирования методом Лагранжа
                    method = true;
                    GetWindowText(hEdt1, val, 20);
                    if (checkStr(val, 0)) {
                        cnt = StrToInt(val);
                        if ((cnt >= 2) && (cnt <= 10)) {
                            hwndLag = CreateWindow("My Class3", "Вввод данных для метода Лагранжа",
                                WS_OVERLAPPED | WS_SYSMENU | WS_MINIMIZEBOX, 500, 150, 400, 260 + ((cnt - 2) * 40), nullptr, nullptr, hInst, nullptr);
                            ShowWindow(hwndLag, 1);
                            UpdateWindow(hwndLag); 
                        }
                        else if (cnt < 0) {
                            cnt = 0;
                            SetWindowText(hEdt1, IntToStr(cnt));
                            MessageBox(hWnd, "Количество не может быть отрицательным", "Ошибка!", MB_ICONERROR);
                        }

                        else if (cnt > 10) {

                            cnt = 0;
                            SetWindowText(hEdt1, IntToStr(cnt));
                            MessageBox(hWnd, "Ограничение точек неболее 10", "Ошибка!", MB_ICONERROR);
                        }
                        else
                            MessageBox(hWnd, "Для интерполирования\nнужно минимум две точки", "Ошибка!", MB_ICONERROR);
                    }
                    else
                        MessageBox(hWnd, "Требуется ввести целое число", "Ошибка!", MB_ICONERROR);
                }
            }
        }
                if (lParam == (LPARAM)hPlus)    // Увеличение счетчика интерполируемых узлов на 1
                {
                    cnt++;
                    SetWindowText(hEdt1, IntToStr(cnt));
                }
                if (lParam == (LPARAM)hMinus && cnt > 0)  // Уменьшение счетчика интерполируемых узлов на 1
                {
                    cnt--;
                    SetWindowText(hEdt1, IntToStr(cnt));
                }
    }
    break;
    case WM_PAINT: // Отрисовка текста в окне
    {
        
        hdc = BeginPaint(hWnd, &ps);
        TextOut(hdc, 15, 30, "Количество координат", 21); // вывод текстовых сообщений


        EndPaint(hWnd, &ps);
    }
    break;
    
    case WM_DESTROY: // Закрытие окна
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}


// Поток окна ввода данных
/*
    Формальные параметры:
    hwnd - дескриптор родительского окна;
    Message - сообщения;
    wparam - дополнительная информация о сообщениях;
    lparam - дополнительная информация о сообщениях.
    Локальные параметры:
    hPX - поле для ввода интерполируемой точки;
    hPY - поле для вывода результата интерполирования;
    hRes - кнопка черчения графика по точкам;
    ps - структра рисования клиентской области окна;
    hdc - дескриптор контекста устройства;
    fgroup - флаг проверки порядка точек по оси X;
    fh - флаг проверки на одинаковый шаг для метода Ньютона;
    frepeat - флаг проверки повторяющихся точек по оси X;
    fIsNum - флаг проверки корректного ввода числа;
    fRange - флаг проверки выхода за область допустимых значений;
    h - шаг между точками по оси X.
*/
LONG WINAPI WndData(HWND hwnd, UINT Message, WPARAM wparam, LPARAM lparam)
{
    static HWND hPX, hPY;
    static HWND hRes;
    PAINTSTRUCT ps;
    HDC hdc;
    bool fgroup = true, fh = true, frepeat = true, fIsNum = true, fRange = true;
    double h = 0;



    // RECT rc = { 0 };

    //SCROLLINFO si = { 0 };

    //static int  cxChar, cxCaps, cyChar, cyClient, VscrollPos;
    //int i;
    //char buffer[1024]; //размер буфера для текста ставь побольше если будет много текста
    //TEXTMETRIC tm;

    switch (Message)
    {
    case WM_CREATE: // Инициализация объектов окна

        /*SetScrollRange(hwnd, SB_VERT, 0, 30 - 1, FALSE);
        SetScrollPos(hwnd, SB_VERT, VscrollPos, TRUE);*/
        hRes = CreateWindow("button", "Вычислить",
            WS_CHILD | WS_VISIBLE | WS_BORDER,
            235, 50, 100, 30, hwnd, 0, hInst, NULL);
        hPX = CreateWindow("edit", "0",
            WS_CHILD | WS_VISIBLE | WS_BORDER | ES_RIGHT, 50, 50, 60, 20,
            hwnd, 0, hInst, NULL);
        hPY = CreateWindowA("static", "0",
            WS_CHILD | WS_VISIBLE |  ES_RIGHT | WS_BORDER, 150, 50, 60, 20,
            hwnd, 0, hInst, NULL);
        for (int i = 0; i < cnt; i++) {

            hX.push_back(CreateWindow("edit", "0",
                WS_CHILD | WS_VISIBLE | WS_BORDER | ES_RIGHT , 50, 100 + (i + 1) * 40, 60, 20,
                hwnd, 0, hInst, NULL));
            hY.push_back(CreateWindow("edit", "0",
                WS_CHILD | WS_VISIBLE | WS_BORDER | ES_RIGHT, 150, 100 + (i + 1) * 40, 60, 20,
                hwnd, 0, hInst, NULL));
        }
        showXY();
       
        /*GetClientRect(hwnd, &rc);
        si.cbSize = sizeof(SCROLLINFO);
        si.fMask = SIF_ALL;
        si.nMin = 0;
        si.nMax = 150 + cnt * 40;
        si.nPage = (rc.bottom - rc.top);
        si.nPos = 0;
        si.nTrackPos = 0;
        SetScrollInfo(hwnd, SB_VERT, &si, true);*/
        break;
    case WM_COMMAND: // Формирование результатов программы по нажатию кнопки
    {
        if (lparam == (LPARAM)hRes) {
            X.clear();
            Y.clear();
            for (int i = 0; i < cnt; i++) {
                GetWindowText(hX[i], val, 20);
                if ((StrToInt(val) > 10) || (StrToInt(val) < -10))
                    fRange = false;

                if (fIsNum)
                    fIsNum = checkStr(val, 1);
                X.push_back(StrToFlo(val));
                GetWindowText(hY[i], val, 20);
                if ((StrToInt(val) > 10) || (StrToInt(val) < -10))
                    fRange = false;
                if (fIsNum)
                    fIsNum = checkStr(val, 1);
                Y.push_back(StrToFlo(val));
                if (!method) {
                    if (i > 0) {
                        if (X[i] < X[i - 1]) {
                            fgroup = false;
                            break;
                        }
                        if (i == 1) {
                            h = X[i] - X[i - 1];
                        }
                        else
                            if (h != (X[i] - X[i - 1])) {
                                fh = false;
                                break;
                            }

                    }
                }
            }
            for (int i = 0; i < cnt; i++) {
                if (count(X.begin(), X.end(), X[i]) > 1) {
                    frepeat = false;
                    break;
                }
            }
            GetWindowText(hPX, val, 20);
            if ((StrToInt(val) > 10) || (StrToInt(val) < -10))
                fRange = false;
            if (fIsNum)
                fIsNum = checkStr(val, 1);
            pX = StrToFlo(val);
            if ((frepeat) && (fIsNum)&&(fRange)) {
                if (method)
                    SetWindowText(hPY, FloToStr(Ln(X, Y, pX)));
                else
                    if ((fgroup) && (fh))
                        SetWindowText(hPY, FloToStr(Pn(X, Y, pX)));
                    else {
                        if (!fgroup)
                            MessageBox(hwnd, "Данные должны быть упорядоченны по оси Х", "Ошибка!", MB_ICONERROR);
                        if (!fh)
                            MessageBox(hwnd, "Данные по оси Х должны отличаться\nдруг от друга с одинковым шагом", "Ошибка!", MB_ICONERROR);
                        
                    }
            }
            else {
                if(!frepeat)
                    MessageBox(hwnd, "Одному и тому же значению X не может\nсоответсвовать несколько значений Y", "Ошибка!", MB_ICONERROR);
                if(!fIsNum)
                    MessageBox(hwnd, "Требуется ввести числа в поля для ввода", "Ошибка!", MB_ICONERROR);
            }
            if (!fRange)
                            MessageBox(hwnd, "Данные должны быть введены в диапазоне от -10 до 10", "Ошибка!", MB_ICONERROR);
            if ((frepeat) && (fgroup) && (fh) && (fIsNum) && (fRange)) {
                hwndGraph = CreateWindow("My Class2", "График",
                    WS_OVERLAPPED | WS_SYSMENU | WS_MINIMIZEBOX, 900, 150, 500, 500, nullptr, nullptr, hInst, nullptr);
                ShowWindow(hwndGraph, 1); 
                UpdateWindow(hwndGraph);     
            }
        }
    }
    break;

    //case WM_VSCROLL:
    //{ 
    //    auto action = LOWORD(wparam);
    //    HWND hScroll = (HWND)lparam;
    //    int pos = -1;
    //    if (action == SB_THUMBPOSITION || action == SB_THUMBTRACK) {
    //        pos = HIWORD(wparam);
    //    }
    //    else if (action == SB_LINEDOWN) {
    //        pos = cyChar + 30;
    //    }
    //    else if (action == SB_LINEUP) {
    //        pos = cyChar - 30;
    //    }
    //    if (pos == -1)
    //        break;
    //    WCHAR buf[20];
    //    SCROLLINFO si = { 0 };
    //    si.cbSize = sizeof(SCROLLINFO);
    //    si.fMask = SIF_POS;
    //    si.nPos = pos;
    //    si.nTrackPos = 0;
    //    SetScrollInfo(hwnd, SB_VERT, &si, true);
    //    GetScrollInfo(hwnd, SB_VERT, &si);
    //    pos = si.nPos;
    //    POINT pt;
    //    pt.x = 0;
    //    cxChar = pos - cyChar;
    //    off = off - cxChar;
    //    pt.y = cxChar;
    //    auto hdc = GetDC(hwnd);
    //    LPtoDP(hdc, &pt, 1);
    //    ReleaseDC(hwnd, hdc);
    //    ScrollWindow(hwnd, 0, -pt.y, NULL, NULL);
    //   //InvalidateRect(hwnd, 0, TRUE);
    //    cyChar = pos;
    //    break;
    //}

    //case WM_SIZE:
    //{
    //    RECT rc = { 0 };
    //    GetClientRect(hwnd, &rc);
    //    SCROLLINFO si = { 0 };
    //    si.cbSize = sizeof(SCROLLINFO);
    //    si.fMask = SIF_ALL;
    //    si.nMin = 0;
    //    si.nMax = 150 + cnt*40;
    //    si.nPage = (rc.bottom - rc.top);
    //    si.nPos = 0;
    //    si.nTrackPos = 0;
    //    SetScrollInfo(hwnd, SB_VERT, &si, true);
    //    break;
    //}

    case WM_PAINT: // Вывод текста в окне
    {
        hdc = BeginPaint(hwnd, &ps);

        


        TextOut(hdc, 15, 10, "Требуется найти:", 17);
        for (int i = 0; i <= 1; i++) {
            TextOut(hdc, 50, 30 + (i * 90), "X:", 2);
            TextOut(hdc, 150, 30 + (i * 90), "Y:", 2);
        }
        TextOut(hdc, 15, 100, "Известно:", 10);
        for (int i = 0; i < cnt; i++) {
            TextOut(hdc, 30, 100 + (i + 1) * 40, IntToStr(i+1), 2);
        }
        EndPaint(hwnd, &ps);

    }
    break;

    case WM_DESTROY: // Закрытие окна
        hX.clear();
        hY.clear();
        DestroyWindow(hwnd);
        break;
    default:
        return DefWindowProc(hwnd, Message, wparam, lparam);

    }

    return 0;
}


// Поток окна черчения графика
/*
    Формальные параметры:
    hwnd - дескриптор родительского окна;
    Message - сообщения;
    wparam - дополнительная информация о сообщениях;
    lparam - дополнительная информация о сообщениях.
    Локальные параметры:
    ps - структра рисования клиентской области окна;
    hdc - дескриптор контекста устройства;
    x - координата по оси X;
    y - координата по оси Y.
*/

LONG WINAPI WndGraph(HWND hwnd, UINT Message, WPARAM wparam, LPARAM lparam)
{
    PAINTSTRUCT ps;
    HDC hdc;
    double x = -10, y = 0;
    switch (Message)
    {

    case WM_PAINT: // Черчение плоскости, осей абцисс и ординат, а также черчение самого графика
    {
        HPEN hpen1 = CreatePen(PS_DASH, 3, BLACK_PEN);
        HPEN hpen2 = CreatePen(PS_DASH, 2, RGB(192,192,192));
        HPEN hpen3 = CreatePen(PS_DASH, 2, RGB(255, 0, 0));
        hdc = BeginPaint(hwnd, &ps);
        SelectObject(hdc, hpen2);
        for (int i = 0; i < 25; i++) {
            MoveToEx(hdc, 0, 12 + i * 20, NULL);
            LineTo(hdc, 500, 12 + i * 20);
            MoveToEx(hdc, 12 + i * 20, 0, NULL);
            LineTo(hdc, 12 + i * 20, 500);
        }
        
        TextOut(hdc, 240 + (-10 * 21), 250, "-10" , 3);
        for (int i = -9; i <= 10; i++)
            TextOut(hdc, 240 + (i * 20), 250, IntToStr(i), 2);
        TextOut(hdc, 220, 235 + (10 * 21), "-10", 3);
        for (int i = -9; i <= 10; i++)
            if (i != 0)
                TextOut(hdc, 230, 250 + (-i* 21), IntToStr(i), 2);
        TextOut(hdc, 470, 260, "X", 1);
        TextOut(hdc, 260, 0, "Y", 1);
        SelectObject(hdc, hpen3);
        if (method)
            y = Ln(X, Y, x);
        else
            y  = Pn(X, Y, x);
       // LineTo(hdc, 252 + i * 20, 250 + -y * 20);
        MoveToEx(hdc, 250 + x * 20, 250 + -y * 20, NULL);
        for (double i = -10; i <= 10; i+=0.1) {
            if (method)
                y = Ln(X, Y, i);
            else
                y = Pn(X, Y, i);
            LineTo(hdc, 252 + i * 20, 250 + -y * 20);
            MoveToEx(hdc, 252 + i * 20, 250 + -y * 20, NULL);
            
        }
        SelectObject(hdc, hpen1);
        MoveToEx(hdc, 0, 250, NULL);
        LineTo(hdc, 500, 250);
        MoveToEx(hdc, 250, 0, NULL);
        LineTo(hdc, 250, 500);
        
        EndPaint(hwnd, &ps);
    }
    break;

    case WM_DESTROY: // Закрытие окна
        ShowWindow(hwnd, WM_CLOSE);
        break;
    default:
        return DefWindowProc(hwnd, Message, wparam, lparam);

    }
    
    return 0;
}