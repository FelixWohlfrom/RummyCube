/*
 * Main.cpp
 *
 *  Created on: 22.02.2012
 *      Author: Felix Wohlfrom
 */

#include "game/Settings.h"
#include "gui/MainWindow.h"

#include <time.h>

#include <QApplication>

#ifdef _DEBUG
    #ifdef _WIN32
        #include <iostream>
        #include <io.h>
        #include <Fcntl.h>
        #include <Windows.h>

        #ifdef _MSC_VER
            // Uncomment this if you installed visual memory
            // leak detector from http://vld.codeplex.com/
            #include <vld.h>
        #endif
    #endif
#endif

int main(int argc, char *argv[])
{
    // initialize random seed
    srand(time(NULL));

    // Enable debug console on win32
    #ifdef _DEBUG
        #ifdef _WIN32
            int hConHandle;
            long long lStdHandle;
            FILE *fp;
            // allocate a console for this app
            AllocConsole();

            // redirect unbuffered STDOUT to the console
            lStdHandle = (long long)GetStdHandle(STD_OUTPUT_HANDLE);
            hConHandle = _open_osfhandle(lStdHandle, _O_TEXT);
            fp = _fdopen(hConHandle, "w");
            *stdout = *fp;
            setvbuf(stdout, NULL, _IONBF, 0);
        #endif
    #endif

    QApplication application(argc, argv);

    MainWindow* mainWindow = new MainWindow();
    mainWindow->show();

    int result = application.exec();

    delete mainWindow;
    Settings::cleanup();

    return result;
}
