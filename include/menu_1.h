#pragma once
int main_Initial_Menu(void) {
	while (1) {
		printf("****************************\n");
		printf("[ Please select the menu ]\n");
		printf("1. Sign in\n");
		printf("2. Sign up\n");
		printf("3. Non-Membership\n");
		printf("4. Exit\n");
		printf("\nEnter: ");
		char menu[101];
		int num;
		if (scanf_s("%[^\n]s", menu, sizeof(menu)) == 0) {
			while (getchar() != '\n');
			printf("The number of characters exceeds or just input Enter.\n");
			continue;
		}

		while (getchar() != '\n');

		//if (menu == "Sign in" || menu == "in" || menu == "1")
		if (strcmp(menu, "Sign in") == 0 || strcmp(menu, "in") == 0 || strcmp(menu, "1") == 0)
		{
			num = 1;
		}
		//else if (menu == "Sign up" || menu == "up" || menu == "2")
		else if (strcmp(menu, "Sign up") == 0 || strcmp(menu, "up") == 0 || strcmp(menu, "2") == 0)
		{
			num = 2;
		}
		//else if (menu == "Non-Membership" || menu == "Non" || menu == "3")
		else if (strcmp(menu, "Non-Membership") == 0 || strcmp(menu, "Non") == 0 || strcmp(menu, "3") == 0)
		{
			num = 3;
		}
		//else if (menu == "Exit" || menu == "E" || menu == "e" || menu == "4")
		else if (strcmp(menu, "Exit") == 0 || strcmp(menu, "E") == 0 || strcmp(menu, "e") == 0 || strcmp(menu, "4") == 0)
		{
			num = 4;
		}
		else {
			printf("*It¡¯s not the correct input. Please check it again.*\n");
			//while (getchar() != '\n');
			continue;
		}
		//while (getchar() != '\n');

		switch (num) {
		case 1:
			return 1;
			break;
		case 2:
			return 2;
			break;
		case 3:
			return 3;
			break;
		case 4:
			return -1;
		}
	}
}
