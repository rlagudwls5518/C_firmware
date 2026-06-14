#include <stdio.h>

void main_2() {

	char str[100];
	int alphaCount[26] = { 0 };

	printf("문자열 입력 >>\n");
	fgets(str, 100, stdin);

	if (str[0] == 'e' && str[1] == 'x' && str[2] == 'i' && str[3] == 't') {
		printf("종료합니다.\n");
		return;
	}

	int i = 0;
	strlwr(str);

	while (str[i] != '\n') {
		
		if (str[i] >= 'a' && str[i] <= 'z') {
			alphaCount[str[i] - 'a']++;
		}
		i++;
	}
	printf("\n");

	printf("문자 출현 횟수와 그래프 : \n");
	printf("\n");
	printf("\n");

	for (int j = 0; j < 26; j++) {
		char ch = j + 'a';
	
		if (alphaCount[j] != 0) {
			printf("%c(%d) ", ch, alphaCount[j]);
			for (int i = 0; i < alphaCount[j]; i++) {
				printf("*");
			}
			printf("\n");
		}
		
	}

}