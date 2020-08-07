/*
 * Copyright Peter Mezei 2020
 * Code is based on ©2015 Symbol Technologies LLC. All rights reserved.
 * I used the demo library installed by Zebra SDK for linux
 */


#include "ConsoleMain.h"
#include "ConsoleSampleEventListener.h"

#include <stdlib.h>
// class LoggingContext;

bool hasAlreadyOpen = false;

int ReturnChoice()
{           
	int choice = 0;
	cin >> choice;
	return choice;
}

int main(void)
{
	SampleEventListener sel;

	sel.Open();
	int choice = ReturnChoice();

	do {
		switch (choice) {
            // OnOkEvent
            case 1:
                break;
            // OnErrorEvent
            case 2:
                sel.executeError();
                break;
            case 0:
                break;
            default:
                break;
		}
        choice = ReturnChoice();

	} while (true);

	sel.Close();
	return 0;
}
