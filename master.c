#include <unistd.h>       // close, read, write 함수 사용
#include <arpa/inet.h>    // htonl, htons, ntonl, ntons 함수 사용
#include <sys/socket.h>   // socket, connect 함수 사용
#include "mod.c"
#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 502

int main()
{
    int cl_sock;
    struct sockaddr_in serv_addr;
    int str_len;


    // 제 1단계 - 소켓 생성(socket 함수 사용)
    cl_sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

    if (cl_sock == -1)
        printf("socket() error");

    memset(&serv_addr, 0, sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(SERVER_IP);
    serv_addr.sin_port = htons( SERVER_PORT );

    // 제 2단계 - 서버 연결
    if(connect(cl_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1)
    {
        printf("connect() error");
        close(cl_sock);
        exit(1);
    }
    else
    {
        printf("connect() seccess!!!\n\n");
    }


    // RTU 구조체 e_rtu 생성 및 값 설정
    // AppDataUnit* setAdu(byte funcCode, short stAdd, byte * len);
    // 0x08(2진수 : 0000 1000) -> 배열의 빅엔디안 0x0800(주소 값이 큰 08이 먼저 오고 00이 뒤로 옴) 배열의 크기는 2 바이트 이다.
    byte * request = NULL;



    // short a_pid = 0x0000;
    // short a_restLength = 0x0006;
    byte slvId = 0x01;
    byte funcCode = 0x03;
    short stAddr = 0x0003;
    // short regCnt = 0x0001;

    // crc16
    byte sendData[10];
    byte * p_crc = NULL;

    byte crc_len;
    p_crc = &sendData[0];
    crc_len = 0x06;

    // crc가 보내는 데이터 패킷(request)의 자료를 할당해야함.(memset 함수로 사용)
    memset(&sendData[0], 0x01, 1);   // 슬레이브 ID 1번
    memset(&sendData[1], 0x03, 1);   // functionCode
    memset(&sendData[2], 0x00, 1);   // 레지스터 시작주소
    memset(&sendData[3], 0x03, 1);
    memset(&sendData[4], 0x00, 1);   // 데이터 길이 6개
    memset(&sendData[5], 0x06, 1);

    request = rtuSetRequest(slvId, funcCode, stAddr, p_crc, crc_len);



    /* 클라이언트 -> 슬레이브 데이터 전송할 때 write 함수 호출
       write 함수 원형
       ssize_t write(int fd, const void *buf, size_t nbytes);
       성공 시 전달한 바이트 수, 실패 시 -1 반환
       fd : 데이터 전송 대상을 나타내는 파일 디스크립터 전달.
       buf : 전송할 데이터가 저장된 버퍼의 주소 값 전달.
       nbytes : 전송할 데이터의 바이트 수 전달. */
    // 클라이언트 -> 슬레이브(서버) 데이터 전송

    int rlen = sizeof(request) / sizeof(byte);
    write(cl_sock, request, rlen);

    printf("[Master] 송신한 request 패킷 : ");

    int j;
    for (j = 0; j < rlen; j++)
    {
        printf("%u ", request[j]);
    }

    printf("\n\n");

    byte response[RESPONSE_LEN];
    /* 슬레이브 -> 클라이언트 데이터 수신할 때 read 함수 호출
       read 함수 원형
       ssize_t read(int fd, void * buf, size_t nbytes);
       성공 시 수신한 바이트 수(단, 파이의 끝을 만나면 0), 실패 시 -1 반환
       fd : 데이터 수신 대상을 나타내는 파일 디스크립터 전달
       buf : 수신한 데이터를 저장할 버퍼의 주소 값 전달
       nbytes : 수신할 최대 바이트 수 전달 */
    // 슬레이브 -> 클라이언트 데이터 수신
    read(cl_sock, response, RESPONSE_LEN);

    int res_len = resLenCheck(response);

    printf("[Master] 수신한 response 패킷 : ");
    int k;
    for (k = 0; k < res_len; k++)
    {
        printf("%u ", response[k]);
    }
    printf("\n\n");


    res_decoder(response, res_len);

    close(cl_sock);

    system("pause");

    return 0;
}
