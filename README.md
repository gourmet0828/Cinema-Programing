# -Cinema-Programing
이 프로그램은 C로 구현 후 컴파일된 .exe 파일 실행을 통해 작동합니다.

2.1 작동 환경

- MS Windows 10, 11의 cmd 창에서 확실히 정상 작동합니다.
- GUI 파일 관리창에서 주 실행 파일을 더블클릭하여 실행하면 정상작동을 보장하지 않습니다.
- 이외 환경에서는 작동할 수 있지만 보장되지는 않습니다.

2.2 프로그램 구성

- 프로그램이 배포될 때에는 주 실행 파일인 main.exe 파일 하나만 배포됩니다.
- 프로그램이 올바르게 실행되면, 홈 경로에 “Member.txt”, “Guest.txt”, “TheaterA.csv”, “TheaterB.csv”, “TheaterC.csv”, “Reservation.csv”, “DateTime.csv”, “LogData.csv” 데이터 파일이 있는지 확인하고 없으면 생성합니다. 즉, 데이터 파일의 전체 경로는 “{HOME}\<데이터파일>”입니다. 이 파일은 사용자가 직접 수동으로 지우기 전까지는 (즉 프로그램 통해서는) 지워지지 않습니다.
- 프로그램 스스로는 위 데이터 파일 외에 다른 파일을 더 생성하지 않습니다. 혹시 exe 파일이 어딘가에 디렉토리나 파일을 생성할 수도 있지만, 이럴 가능성은 낮습니다.

2.3 프로그램 설치 및 실행

- 아무 경로에나 프로그램 주 실행 파일을 복사하여 설치합니다.
- cmd 창에서 주 실행 파일의 절대 경로를 입력하여 프로그램 홈 경로로 이동합니다.

<aside>
💡 C: \Users\User> cd C:\Users\User\Desktop\Cinema

</aside>

- 주 실행  파일의 파일명을 입력하여 프로그램을 실행합니다.

<aside>
💡 C:\Users\User\Desktop\Cinema>main.exe

</aside>

