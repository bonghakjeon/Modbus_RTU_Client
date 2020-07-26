//
// Created by wjsqh on 2020-07-19.
//


#include <stdio.h>    // sizeof 연산자 사용
#include <stdlib.h>   // calloc, free, system 함수 사용
#include <string.h>   // memset, memcpy 함수 사용
#include "mod.h"



// -----------------------------------------------------------------------------------------------------------

// MBAPHeader 관련 함수 4개 + 1개

// MBAPHeader 생성 및 초기화 시켜주는 함수 MkMbHeader_init
MBAPHeader* MkMbHeader_init(void)
{
    // void* calloc(size_t elt_count, size_t elt_size);
    // elt_count : 할당할 블록의 갯수 정보, elt_size : 블록 하나당 바이트 크기의 정보
    MBAPHeader * mbHeader = NULL;

    mbHeader = (MBAPHeader *)calloc(sizeof(MBAPHeader), sizeof(byte));

    return mbHeader;
}

// MBAPHeader를 byte 배열 형태로 반환해주는 함수 getHeader
byte* getHeader(MBAPHeader * mbHeader)
{
    return mbHeader->header;
}

// 인자로 전달된 a_header를 구조체 MBAPHeader의 멤버변수 header에 저장하는 함수 setMbHeader
void setMbHeader(MBAPHeader * mbHeader, byte * a_header)
{
    memcpy(mbHeader->header, a_header, sizeof(mbHeader->header));
}

// 인자로 전달된 a_mbHeader에 저장된 내용(배열)을 출력 해주는 함수 ShowInfoMbHeader
void ShowInfoMbHeader(MBAPHeader * a_mbHeader)
{
    int i;
    for (i = 0; i < sizeof(a_mbHeader->header) / sizeof(byte); i++)
    {
        printf("%u", a_mbHeader->header[i]);   // %u : 부호 없는 10진수 정수
    }
    printf("\n\n");
}



// -----------------------------------------------------------------------------------------------------------


// AppDataUnit 관련 함수 12개

// AppDataUnit 생성 및 초기화 시켜주는 함수 MkAppDataUnit_init
AppDataUnit* MkAppDataUnit_init(void)
{
    AppDataUnit * adu = NULL;

    adu = (AppDataUnit *)calloc(sizeof(AppDataUnit), sizeof(byte));

    return adu;
}

// RTU 생성 및 초기화 시켜주는 함수 MkRTU_init
RTU* MkRTU_init(void)
{
    RTU * rtu = NULL;

    rtu = (RTU *)calloc(sizeof(RTU), sizeof(byte));

    return rtu;
}

// TCP 생성 및 초기화 시켜주는 함수 MkTCP_init
TCP* MkTCP_init(void)
{
    TCP * tcp = NULL;

    tcp = (TCP *)calloc(sizeof(TCP), sizeof(byte));

    return tcp;
}

// AppDataUnit 구조체 변수를 인자로 받아서 구조체 AppDataUnit의 멤버에 값을 저장해 준 후 ADU 구조체 변수를 반환하는 함수 AppDataUnit_set
AppDataUnit* AppDataUnit_set(MBAPHeader * mbapHeader, byte a_funcCode, byte * stAddr, byte * len)
{
    // AppDataUnit 생성 및 초기화
    AppDataUnit * adu = NULL;

    adu = MkAppDataUnit_init();

    // AppDataUnit 값 설정(memcpy 함수 사용)
    memcpy(adu->mbHeader.header, mbapHeader->header, sizeof(adu->mbHeader.header));
    adu->functionCode = a_funcCode;
    memcpy(adu->startAddr, stAddr, sizeof(adu->startAddr));
    memcpy(adu->a_dataCnt, len, sizeof(adu->a_dataCnt));

    return adu;
}

// 슬레이브에서 응답받은 byte 배열 response를 인자로 받아서 ADU 구조체 변수에 저장해준 후에 ADU 구조체 변수를 반환하는 AppDataUnit_Slave
AppDataUnit* AppDataUnit_Slave(byte * response)
{
    // byte 배열 temp 선언
    byte temp[MBHEADER_LEN];

    // byte 배열 temp 초기화
    memset(temp, 0, sizeof(temp));

    int i = 0, j = 0;

    for (int i = 0; i < sizeof(temp) / sizeof(byte); i++)
    {
        temp[i] = response[i];
    }

    // AppDataUnit 생성 및 초기화
    AppDataUnit * adu = NULL;

    adu = MkAppDataUnit_init();


    // 인자로 전달된 mbHeader를 구조체 MBAPHeader의 멤버변수 header에 저장
    // void setMbHeader(MBAPHeader * mbHeader, byte * a_header);
    setMbHeader(&(adu->mbHeader), temp);

    adu->functionCode = response[7];

    // byte 배열 addr_temp 선언
    byte addr_temp[TWO_BYTES];

    // byte 배열 addr_temp 초기화
    memset(addr_temp, 0, sizeof(addr_temp));

    addr_temp[0] = response[8];
    addr_temp[1] = response[9];

    // memcpy 함수 원형
    // void* memcpy(void* destination, const void* source, size_t num);
    memcpy(adu->startAddr, addr_temp, sizeof(adu->startAddr));

    // byte 배열 len 선언(배열의 크기 : TO_BYTES_LEN(매크로 상수) - 10)
    byte len[TO_BYTES_LEN-10];

    // byte 배열 len 초기화
    memset(len, 0, sizeof(len));

    for (int j = 10; j < (sizeof(response) / sizeof(byte)); i++)
    {
        len[i - 10] = response[i];
    }

    memcpy(adu->a_dataCnt, len, sizeof(adu->a_dataCnt));

    return adu;
}

// AppDataUnit 구조체 변수를 인자로 받아서 MBAPHeader로 반환해주는 함수 getMbHeader
MBAPHeader* getMbHeader(AppDataUnit * adu)
{
    return &(adu->mbHeader);
}

// byte 배열 a_header를 인자로 받아서 구조체 AppDataUnit의 멤버 변수인 mbapHeader에 값을 저장해 주는 함수 setMbHeader
void setADUHeader(AppDataUnit * adu, byte * a_header)
{
    memcpy(adu->mbHeader.header, a_header, sizeof(adu->mbHeader.header));
}

// AppDataUnit 구조체 변수를 인자로 받아서 구조체의 멤버인 functionCode에 저장된 값을 반환해주는 함수 getFunctionCode
byte getFunctionCode(AppDataUnit * adu)
{
    return adu->functionCode;
}

// AppDataUnit 구조체 변수와 a_funcCode를 인자로 받아서 구조체 AppDataUnit의 멤버 변수인 functionCode에 값을 저장해 주는 함수 setFunctionCode
void setFunctionCode(AppDataUnit * adu, byte a_funcCode)
{
    adu->functionCode = a_funcCode;
}

/* AppDataUnit 구조체 변수를 인자로 받아서 구조체의 멤버인 바이트 배열 startAddr에 저장된 값을 byte 배열 형태로 반환해주는 함수 getStartAddr */
byte* getStartAddr(AppDataUnit * adu)
{
    return adu->startAddr;
}

/* byte 배열 startAddress를 인자로 받아서 구조체 AppDataUnit의 멤버인 바이트 배열 startAddr에 값을 저장해 주는 함수 setStartAddr */
void setStartAddr(AppDataUnit * adu, byte * startAddress)
{
    memcpy(adu->startAddr, startAddress, sizeof(adu->startAddr));
}

/* AppDataUnit 구조체 변수를 인자로 받아서 구조체 AppDataUnit의 멤버인 바이트 배열 length에 저장된 값을
   byte 배열 형태로 반환해주는 함수(상위바이트/ 하위바이트로 나눠서 길이값을 비트 값으로 표현함.) */
byte* getLength(AppDataUnit * adu)
{
    return adu->a_dataCnt;
}

/* byte 배열 len을 인자로 받아서 구조체 AppDataUnit의 멤버인 바이트 배열 length에 값을 저장해 주는 함수 setLength */
void setLength(AppDataUnit * adu, byte * len)
{
    memcpy(adu->a_dataCnt, len, sizeof(adu->a_dataCnt));
}

/* AppDataUnit 구조체 변수를 인자로 받아서 구조체 AppDataUnit에 저장된 내용을 출력해주는 함수 ShowInfoAppDataUnit */
void ShowInfoAppDataUnit(AppDataUnit * adu)
{
    int i = 0;

    printf("mbapHeader = MBAPHeader [header = [ ");

    for (i = 0; i < (sizeof(adu->mbHeader.header) / sizeof(byte)) - 1; i++)
    {
        printf(" %u,", adu->mbHeader.header[i]);
    }
    printf(" %u ]], ", adu->mbHeader.header[6]);

    printf("FunctionCode : %u, ", adu->functionCode);


    printf("startAddr = [");

    printf(" %u,", adu->startAddr[0]);   // %u : 부호 없는 10진수 정수
    printf(" %u ", adu->startAddr[1]);

    printf(" ], ");


    printf("Length = [");

    printf(" %u,", adu->a_dataCnt[0]);
    printf(" %u ", adu->a_dataCnt[1]);

    printf(" ]]");
}


// ----------------------------------------------------------------------------------------------------------


// Modbus 관련 함수 5개
// AppDataUnit 구조체형 변수 형태로 요청할(Parameter) 값을 셋팅해 주는 함수 setAdu
AppDataUnit* setAdu(byte * a_header, byte a_funcCode, short stAddr, byte * len)
{
    // byte 배열 temp 선언 및 초기화
    byte temp[TWO_BYTES] = { 00, 00 };


    // MBAPHeader 생성 및 초기화
    MBAPHeader * p_header = NULL;

    p_header = MkMbHeader_init();


    // MBAPHeader 값 설정
    // void setMbHeader(MBAPHeader * mbHeader, byte * a_header)
    setMbHeader(p_header, a_header);

    // memcpy 함수 사용
    memcpy(temp, shortToBytes(stAddr), sizeof(temp));

    // byte 배열 startAddress 선언 및 초기화
    byte startAddress[TWO_BYTES] = { 00, 00 };

    memcpy(startAddress, temp, sizeof(startAddress));


    // AppDataUnit 생성 및 값 설정
    // AppDataUnit_set함수 호출시 MkAppDataUnit_init 함수가 호출되어 초기화 됨.
    // 그래서 p_adu에 생성 및 초기화 함수를 호출하면 에러 발생함.
    AppDataUnit * p_adu = NULL;

    p_adu = AppDataUnit_set(p_header, a_funcCode, startAddress, len);

    return p_adu;
}


// byte 배열 len을 인자로 받아 구조체 변수 MBAPHeader 값을 셋팅해주는 함수 setHeader
MBAPHeader* setHeader(byte * a_header)
{
    // byte 배열 temp 선언 및 초기화
    byte temp[TWO_BYTES] = { 00, 00 };

    // byte 배열 header_Temp 선언
    byte header_Temp[MBHEADER_LEN];

    // byte 배열 header_Temp 초기화
    memset(header_Temp, 0, sizeof(header_Temp));

    // 1. MBAP header_Temp 셋팅
    // transactionId = transactionId + 0x01; 과 같음.
    transactionId += 0x01;

    // memcpy 함수 사용
    memcpy(temp, shortToBytes((short)transactionId), sizeof(temp));

    // 배열 header_Temp에 값 할당
    header_Temp[0] = temp[0];
    header_Temp[1] = temp[1];

    // Protocol Id - MODBUS-TCP 0x0000으로 고정 셋팅
    header_Temp[2] = 0x0000;
    header_Temp[3] = 0x0000;

    /* len - UnitId ~ Data
       len 필드 위치에서 프레임 마지막까지의 길이를 나타냄.
       즉, UnitID ~ Data 끝까지의 Byte 수를 나타냄. */
    // 4 - unitId ~ startAddress 바이트 길이
    short length_Temp = (short)(Unit_FC_StAddr + (sizeof(a_header) / sizeof(byte)));

    memcpy(temp, shortToBytes(length_Temp), sizeof(temp));

    header_Temp[4] = temp[0];
    header_Temp[5] = temp[1];

    // Unit Id = TCP port에서는 0x01로 고정
    header_Temp[6] = 0x01;

    // MBAPHeader 생성 및 초기화
    MBAPHeader * p_header = NULL;

    p_header = MkMbHeader_init();


    // MBAPHeader 값 설정(인자값 header_Temp)
    // void setMbHeader(MBAPHeader * mbHeader, byte * a_header)
    setMbHeader(p_header, header_Temp);

    return p_header;
}


// 마스터에서 Request한 값을 슬레이브에서 byte 배열 형태로 변환해주는 함수 encoder
byte* encoder(AppDataUnit * adu)
{
    // encoder 함수에 사용하기 위해 전역 변수(배열)로 선언한 배열 toBytes(배열의 길이 12) 초기화
    // byte toBytes[TO_BYTES_LEN];
    memset(toBytes, 0, sizeof(toBytes));

    // MBAPHeader 생성 및 초기화
    MBAPHeader * p_header = NULL;

    p_header = MkMbHeader_init();

    // MBAPHeader 값 설정
    // void setMbHeader(MBAPHeader * mbHeader, byte * a_header)
    setMbHeader(p_header, adu->mbHeader.header);


    // memcpy 함수를 사용하여 toBytes 배열에 내용 복사
    // 0~6번지(7 바이트) MBAPHeader 복사
    memcpy(toBytes, p_header, sizeof(p_header));

    int i = 0;

    for (int i = 0; i < (sizeof(p_header->header) / sizeof(byte)); i++)
    {
        toBytes[i] = (p_header->header)[i];
    }


    // 배열처럼 인덱스로 접근하여 7번지(1 바이트)에 FunctionCode의 값 할당
    toBytes[7] = adu->functionCode;

    // 배열처럼 인덱스로 접근하여 8번지(1 바이트)에 StartAddr의 상위바이트 값 할당
    toBytes[8] = (adu->startAddr)[0];


    // 배열처럼 인덱스로 접근하여 9번지(1 바이트)에 StartAddr의 하위바이트 값 할당
    toBytes[9] = (adu->startAddr)[1];

    // for문을 사용하여 toBytes 배열 10번지 이후부터 각 번지 값 저장하기
    // byte* getLength(AppDataUnit * adu)
    int j = 0;

    for (int j = 10; j < (sizeof(toBytes) / sizeof(byte)); j++)
    {
        // 배열처럼 인덱스로 접근하여 값 할당
        toBytes[j] = (adu->a_dataCnt)[j - 10];
    }

    return toBytes;
}



// 슬레이브에서 Response 받은 값을 마스터에서 byte 배열로 반환해주는 함수 decoder
byte* decoder(byte* response)
{
    // AppDataUnit 생성 및 초기화
    AppDataUnit * p_adu = NULL;

    p_adu = MkAppDataUnit_init();


    // for문에 사용할 int형 변수 i, j 설정
    int i = 0;
    int j = 0;

    // AppDataUnit 값 설정(인자 값 response)
    // 1단계 MBAPHeader mbHeader 값 설정
    for (int i = 0; i < (sizeof(p_adu->mbHeader.header) / sizeof(byte)); i++)
    {
        p_adu->mbHeader.header[i] = response[i];
    }

    // 2단계 functionCode 값 설정
    p_adu->functionCode = response[7];

    // 3단계 startAddr 값 설정
    p_adu->startAddr[0] = response[8];
    p_adu->startAddr[1] = response[9];

    // 4단계 length 값 설정
    p_adu->a_dataCnt[0] = response[10];
    p_adu->a_dataCnt[1] = response[11];


    // 전역 배열 toBytes 초기화
    memset(toBytes, 0, sizeof(toBytes));

    // 제 1단계 전역 배열 toBytes에 MBAPHeader mbHeader 값 할당
    for (int j = 0; j < (sizeof(p_adu->mbHeader.header) / sizeof(byte)); j++)
    {
        toBytes[j] = p_adu->mbHeader.header[j];
    }

    // 제 2단계 전역 배열 toBytes에 functionCode 값 할당
    toBytes[7] = p_adu->functionCode;


    // 제 3단계 전역 배열 toBytes에 startAddr 값 할당
    toBytes[8] = p_adu->startAddr[0];
    toBytes[9] = p_adu->startAddr[1];

    // 제 4단계 전역 배열 toBytes에 length 값 할당
    toBytes[10] = p_adu->a_dataCnt[0];
    toBytes[11] = p_adu->a_dataCnt[1];

    return toBytes;
}


// short 형태로 받은 자료형을 byte 배열 형태로 변환 해주는 함수 shortToBytes
// short 형태로 받은 자료형을 printf 함수를 사용하여 상위바이트, 하위바이트 출력
byte* shortToBytes(short value)
{
    // STBBuf_t 공용체 변수 buf 선언
    STBBuf_t buf;
    buf.iBuf = value;
    /*
       printf("상위 바이트 : %u\n", buf.stbBuf.upper);
       printf("하위 바이트 : %u\n", buf.stbBuf.lower);
     */

    // shortToBytes함수에 사용하기 위해 배열 ul_Array의 동적 할당 및 초기화
    byte * ul_Array = NULL;

    ul_Array = (byte *)calloc(sizeof(byte) * TWO_BYTES, sizeof(byte));

    // 전역 변수(배열)로 선언한 배열 ul_Array의 각 번지에 값 할당
    ul_Array[0] = buf.stbBuf.upper;  // 상위 1바이트 값 저장
    ul_Array[1] = buf.stbBuf.lower;  // 하위 1바이트 값 저장

    return ul_Array;
}

/* byte 형태로 받은 배열을 short형 자료형 변수로 변환 해주는 함수 bytesToShort */
short bytesToShort(byte * param)
{
    byte temp[TWO_BYTES] = { 00, 00 };

    memcpy(temp, param, sizeof(temp));

    short value = 0;

    // 빅 엔디안 방식
    value = *(temp) << 8 | *(temp+1);

    /* 리틀 엔디안 방식
    value = *(temp+1) << 8 | *(temp); */

    return value;

}


//=========================================================================//
// CRC16 을 BYTE[2] 로 리턴하는 함수 crc16_ShortToBytes
//=========================================================================//

byte* crc16_ShortToBytes(byte * a_crc16, int ilen)
{
    // ErrCheck 공용체 변수 crc16 선언.
    ErrCheck_t crc16;

    crc16.icrc = 0xFFFF;
    // printf("The two bytes: %d and %d\n", crc16.b_num[0], crc16.b_num[1]);

    int i;


    for (int i = 0; i < ilen; i++)
    {
        crc16.icrc = (crc16.icrc >> 8) ^ CRC_Table[(crc16.icrc ^ a_crc16[i]) & 0xff];
    }

    // ErrCheck 공용체 변수 crc16의 멤버 icrc에 저장된 값을 바이트 배열로 변환함.

    // 배열 ret의 동적 할당 및 초기화
    byte * ret = NULL;

    ret = (byte*)calloc(sizeof(byte) * TWO_BYTES, sizeof(byte));

    ret[0] = crc16.b_num[0];
    ret[1] = crc16.b_num[1];

    return ret;
}


// main 함수에서 Response를 출력하는 함수 ShowInfo_Res
void ShowInfo_Res(AppDataUnit * res_adu)
{
    int i;
    int j;
    int k;

    for (int i = 0; i < 7; i++)
    {
        printf("%u ", res_adu->mbHeader.header[i]);

    }

    printf("%u ", res_adu->functionCode);

    for (int j = 0; j < 2; j++)
    {
        printf("%u ", res_adu->startAddr[j]);

    }

    for (int k = 0; k < 2; k++)
    {
        printf("%u ", res_adu->a_dataCnt[k]);
    }

}

// RTU request 데이터 패킷으로 셋팅해주는 함수 rtuSetRequest
byte* rtuSetRequest(byte slvID, byte funcCode, short regStAddr, byte * a_crc16, unsigned int crc_len)
{
    // byte 배열 temp 선언 및 초기화
    byte temp[TWO_BYTES] = { 00, 00 };

    // RTU 구조체 생성 및 초기화
    RTU * rtu = NULL;

    rtu = MkRTU_init();


    // 슬레이브 주소 (request & response 공용)
    rtu->slaveAddr = slvID;

    // functionCode (request & response 공용)
    rtu->functionCode = funcCode;

    // 레지스터 시작 주소 (request & response 공용)
    rtu->regStartAddr = regStAddr;

    /* RTU 구조체 멤버 데이터 갯수(rtu->dataCnt)


       RTU 구조체 멤버 제어 상태 값(rtu->ctrlValue)


       RTU 구조체 멤버 제어 레지스터 수(rtu->ctrlRegCnt)


       RTU 구조체 멤버 데이터 바이트 수(rtu->d_byteCnt)


       RTU 구조체 멤버 제어 Data 배열(rtu->ctrlData)


       RTU 구조체 멤버 request 배열(rtu->request) */

    // 레지스터 40002
    short reg40002;

    // 레지스터 40003
    short reg40003;


    // 0x01, 0x04, 0x05, 0x0F, 0x10을 매크로 상수로 바꾸기
    // request 데이터 패킷의 길이가 8인 경우(0x01, 0x04, 0x05)
    if (rtu->functionCode == 0x01)
    {
        rtu->request = NULL;

        rtu->request = (byte*)calloc(8, sizeof(byte));

        printf("Digital Input 명령인 READ_COIL 입니다.(0x01)\n\n");

        printf("디지털 데이터의 갯수 입력 : ");


        scanf("%hd", &(rtu->dataCnt));

        rtu->request[0] = rtu->slaveAddr;
        rtu->request[1] = rtu->functionCode;
        rtu->request[2] = shortToBytes(rtu->regStartAddr)[0];
        rtu->request[3] = shortToBytes(rtu->regStartAddr)[1];
        rtu->request[4] = shortToBytes(rtu->dataCnt)[0];
        rtu->request[5] = shortToBytes(rtu->dataCnt)[1];

        // memcpy 함수 사용(crc16_ShortToBytes(a_crc16, ilen))
        memcpy(temp, crc16_ShortToBytes(a_crc16, crc_len), sizeof(temp));

        rtu->request[6] = temp[0];
        rtu->request[7] = temp[1];

        // memset 함수 사용
        memset(temp, 0, sizeof(temp));

    }

    else if (rtu->functionCode == 0x04)
    {
        rtu->request = NULL;

        rtu->request = (byte*)calloc(8, sizeof(byte));

        printf("아날로그 / 디지털 입력포트 확인 명령인 READ_INPUT_REGISTER 입니다.(0x04)\n\n");

        printf("디지털 데이터의 갯수 입력 : ");

        scanf("%hd", &(rtu->dataCnt));

        rtu->request[0] = rtu->slaveAddr;
        rtu->request[1] = rtu->functionCode;
        rtu->request[2] = shortToBytes(rtu->regStartAddr)[0];
        rtu->request[3] = shortToBytes(rtu->regStartAddr)[1];
        rtu->request[4] = shortToBytes(rtu->dataCnt)[0];
        rtu->request[5] = shortToBytes(rtu->dataCnt)[1];

        // memcpy 함수 사용(crc16_ShortToBytes(a_crc16, ilen))
        memcpy(temp, crc16_ShortToBytes(a_crc16, crc_len), sizeof(temp));

        rtu->request[6] = temp[0];
        rtu->request[7] = temp[1];

        // memset 함수 사용
        memset(temp, 0, sizeof(temp));
    }

    else if (rtu->functionCode == 0x05)
    {
        rtu->request = NULL;

        rtu->request = (byte*)calloc(8, sizeof(byte));

        printf("(0x05)\n\n");

        printf("제어 상태 값 입력 : ");

        scanf("%hd", &(rtu->a_ctrlValue));

        rtu->request[0] = rtu->slaveAddr;
        rtu->request[1] = rtu->functionCode;
        rtu->request[2] = shortToBytes(rtu->regStartAddr)[0];
        rtu->request[3] = shortToBytes(rtu->regStartAddr)[1];
        rtu->request[4] = shortToBytes(rtu->a_ctrlValue)[0];
        rtu->request[5] = shortToBytes(rtu->a_ctrlValue)[1];

        // memcpy 함수 사용(crc16_ShortToBytes(a_crc16, ilen))
        memcpy(temp, crc16_ShortToBytes(a_crc16, crc_len), sizeof(temp));

        rtu->request[6] = temp[0];
        rtu->request[7] = temp[1];

        // memset 함수 사용
        memset(temp, 0, sizeof(temp));

    }

        // request 데이터 패킷의 길이가 10인 경우(0x0F)
    else if (rtu->functionCode == 0x0F)
    {
        rtu->request = NULL;

        rtu->request = (byte*)calloc(10, sizeof(byte));

        printf("(0x0F)\n\n");


        // 제어 레지스터 수(short ctrlRegCnt)
        printf("제어 레지스터 갯수 입력 : ");

        scanf("%hd", &(rtu->ctrlRegCnt));

        printf("\n\n");

        // Data 바이트 수(byte d_byteCnt)
        printf("Data 바이트 수 입력 : ");

        scanf("%c", &(rtu->d_byteCnt));

        printf("\n\n");


        // 제어 상태값(short ctrlValue)
        printf("제어 상태값 입력 : ");

        scanf("%c", &(rtu->ctrlValue));


        // RTU 구조체 request 배열에 값 할당
        rtu->request[0] = rtu->slaveAddr;      // 슬레이브 주소
        rtu->request[1] = rtu->functionCode;   // functionCode
        rtu->request[2] = shortToBytes(rtu->regStartAddr)[0]; // 레지스터 시작 주소 Hi
        rtu->request[3] = shortToBytes(rtu->regStartAddr)[1]; // 레지스터 시작 주소 Lo
        rtu->request[4] = shortToBytes(rtu->ctrlRegCnt)[0];   // 제어 레지스터 수 Hi
        rtu->request[5] = shortToBytes(rtu->ctrlRegCnt)[1];   // 제어 레지스터 수 Lo
        rtu->request[6] = (rtu->d_byteCnt);    // Data 바이트 수
        rtu->request[7] = (rtu->ctrlValue);    // 제어 상태값


        // memcpy 함수 사용(crc16_ShortToBytes(a_crc16, ilen))
        memcpy(temp, crc16_ShortToBytes(a_crc16, crc_len), sizeof(temp));

        rtu->request[8] = temp[0];   // CRC 코드 Hi
        rtu->request[9] = temp[1];   // CRC 코드 Lo

        // memset 함수 사용
        memset(temp, 0, sizeof(temp));


    }

        // request 데이터 패킷의 길이가 13인 경우(0x10)
    else if (rtu->functionCode == 0x10)
    {
        rtu->request = NULL;

        rtu->request = (byte*)calloc(13, sizeof(byte));

        printf("(0x10)\n\n");

        // 레지스터 40002(short reg40002)
        printf("레지스터 40002 제어 Data 값 입력 : ");

        scanf("%hd", &reg40002);

        printf("\n\n");


        // 레지스터 40003(short reg40003)
        printf("레지스터 40003 제어 Data 값 입력 : ");

        scanf("%hd", &reg40003);

        printf("\n\n");

        // 제어 레지스터 수(short ctrlRegCnt)
        printf("제어 레지스터 갯수 입력 : ");

        scanf("%hd", &(rtu->ctrlRegCnt));

        // Data 바이트 수(byte d_byteCnt)
        printf("Data 바이트 수 입력 : ");

        scanf("%c", &(rtu->d_byteCnt));

        if (rtu->d_byteCnt != 04)
        {
            printf("d_byteCnt에 값을 잘못 입력하였습니다.");
            exit(1);
        }


        // 제어 Data 배열(calloc으로 동적 할당)
        rtu->ctrlData = (byte*)calloc(sizeof(byte)*(rtu->d_byteCnt), sizeof(byte));


        // memcpy 함수 사용(shortToBytes(reg40002))
        memcpy(temp, shortToBytes(reg40002), sizeof(temp));


        rtu->ctrlData[0] = temp[0];
        rtu->ctrlData[1] = temp[1];


        // memset 함수 사용
        memset(temp, 0, sizeof(temp));

        // memcpy 함수 사용(shortToBytes(reg40003))
        memcpy(temp, shortToBytes(reg40003), sizeof(temp));

        rtu->ctrlData[2] = temp[0];
        rtu->ctrlData[3] = temp[1];


        // memset 함수 사용
        memset(temp, 0, sizeof(temp));



        // RTU 구조체 request 배열에 값 할당
        rtu->request[0] = rtu->slaveAddr;                       // 슬레이브 주소
        rtu->request[1] = rtu->functionCode;                    // functionCode
        rtu->request[2] = shortToBytes(rtu->regStartAddr)[0];   // 레지스터 시작주소 Hi
        rtu->request[3] = shortToBytes(rtu->regStartAddr)[1];   // 레지스터 시작주소 Lo

        rtu->request[4] = shortToBytes(rtu->ctrlRegCnt)[0];     // 제어 레지스터 수 Hi
        rtu->request[5] = shortToBytes(rtu->ctrlRegCnt)[1];     // 제어 레지스터 수 Lo

        rtu->request[6] = rtu->d_byteCnt;                       // Data 바이트 수

        rtu->request[7] = rtu->ctrlData[0];   // 제어 Data Hi (레지스터 40002)
        rtu->request[8] = rtu->ctrlData[1];   // 제어 Data Lo (레지스터 40002)
        rtu->request[9] = rtu->ctrlData[2];   // 제어 Data Hi (레지스터 40003)
        rtu->request[10] = rtu->ctrlData[3];  // 제어 Data Lo (레지스터 40003)


        // memcpy 함수 사용(crc16_ShortToBytes(a_crc16, ilen))
        memcpy(temp, crc16_ShortToBytes(a_crc16, crc_len), sizeof(temp));


        rtu->request[11] = temp[0];  // CRC 코드 Hi
        rtu->request[12] = temp[1];  // CRC 코드 Lo

        // memset 함수 사용
        memset(temp, 0, sizeof(temp));

    }

    else
    {
        printf("functionCode 값을 잘못 입력 하였습니다!!!\n");
        printf("시스템을 종료 합니다!!\n\n");
        exit(1);
    }


    return rtu->request;

}

// RTU response 데이터 패킷으로 셋팅해주는 함수 rtuSetResponse
byte* rtuSetResponse(byte * request, int rlen)
{
    /* response 패킷 구성 */
    /* 기능코드(functionCode에 따라서 구성함.) */

    // byte 배열 temp 선언 및 초기화
    byte temp[TWO_BYTES] = { 00, 00 };


    // RTU 구조체 생성 및 초기화
    RTU * rtu = NULL;

    rtu = MkRTU_init();


    // 제 1단계 RTU 구조체 멤버 slaveID에 값 할당
    rtu->slaveAddr = request[0];

    // 제 2단계 RTU 구조체 멤버 functionCode에 값 할당
    rtu->functionCode = request[1];


    // 레지스터 30001
    short reg30001;

    // 레지스터 30002
    short reg30002;


    // functionCode 값 별로 if~else 구분함.
    if (rtu->functionCode == 0x01)
    {
        printf("functionCode 값이 0x01일 경우 response 패킷을 구성합니다.\n\n");
        // response 배열(calloc으로 동적 할당)
        rtu->response = NULL;

        rtu->response = (byte*)calloc(8, sizeof(byte));

        // 데이터 바이트 수 입력
        printf("Data 바이트 수 입력 : ");

        scanf("%c", &(rtu->d_byteCnt));

        if (rtu->d_byteCnt != 03)
        {
            printf("d_byteCnt에 값을 잘못 입력하였습니다.");
            exit(1);
        }

        // Data 배열(calloc으로 동적 할당)
        rtu->arr_data = (byte*)calloc(sizeof(rtu->d_byteCnt) * 3, sizeof(byte));

        rtu->arr_data[0] = 0xCD;
        rtu->arr_data[1] = 0x6B;
        rtu->arr_data[2] = 0x02;

        rtu->response[0] = rtu->slaveAddr;
        rtu->response[1] = rtu->functionCode;
        rtu->response[2] = rtu->d_byteCnt;

        rtu->response[3] = rtu->arr_data[0];
        rtu->response[4] = rtu->arr_data[1];
        rtu->response[5] = rtu->arr_data[2];


        if (rlen == 8)
        {
            // crc 코드 값 할당
            rtu->crcValue[0] = request[6];
            rtu->crcValue[1] = request[7];


            rtu->response[6] = rtu->crcValue[0];
            rtu->response[7] = rtu->crcValue[1];

        }
        else
        {
            printf("request 패킷 구조가 잘못 되었습니다.\n패킷을 다시 구성하세요.\n");
            exit(1);
        }

    }

    else if (rtu->functionCode == 0x04)
    {
        printf("functionCode 값이 0x04일 경우 response 패킷을 구성합니다.\n\n");
        // response 배열(calloc으로 동적 할당)
        rtu->response = NULL;

        rtu->response = (byte*)calloc(9, sizeof(byte));

        // 데이터 바이트 수 입력
        printf("Data 바이트 수 입력 : ");

        scanf("%c", &(rtu->d_byteCnt));

        if (rtu->d_byteCnt != 04)
        {
            printf("Data 바이트 수에 값을 잘못 입력하였습니다.");
            exit(1);
        }

        // 레지스터 30001 값 입력
        printf("레지스터 30001 제어 Data 값 입력 : ");
        scanf("%hd", &reg30001);

        printf("\n\n");

        // 레지스터 30002 값 입력
        printf("레지스터 30002 제어 Data 값 입력 : ");
        scanf("%hd", &reg30002);

        printf("\n\n");

        // Data 배열(calloc으로 동적 할당)
        rtu->arr_data = (byte*)calloc(sizeof(rtu->d_byteCnt) * 4, sizeof(byte));

        // memcpy 함수 사용(shortToBytes(reg30001))
        memcpy(temp, shortToBytes(reg30001), sizeof(temp));

        rtu->arr_data[0] = temp[0];
        rtu->arr_data[1] = temp[1];

        // memset 함수 사용
        memset(temp, 0, sizeof(temp));

        // memcpy 함수 사용(shortToBytes(reg30002))
        memcpy(temp, shortToBytes(reg30002), sizeof(temp));

        rtu->arr_data[2] = temp[0];
        rtu->arr_data[3] = temp[1];

        // memset 함수 사용
        memset(temp, 0, sizeof(temp));


        // RTU 구조체 response 배열에 값 할당
        rtu->response[0] = rtu->slaveAddr;
        rtu->response[1] = rtu->functionCode;

        rtu->response[2] = rtu->d_byteCnt;

        rtu->response[3] = rtu->arr_data[0];
        rtu->response[4] = rtu->arr_data[1];
        rtu->response[5] = rtu->arr_data[2];
        rtu->response[6] = rtu->arr_data[3];

        // request 배열의 길이 다시 구해야 함.
        if (rlen == 8)
        {

            // CRC 코드값 할당
            rtu->crcValue[0] = request[6];
            rtu->crcValue[1] = request[7];

            rtu->response[7] = rtu->crcValue[0];
            rtu->response[8] = rtu->crcValue[1];

        }
        else
        {
            printf("request 패킷 구조가 잘못 되었습니다.\n패킷을 다시 구성하세요.\n");
            exit(1);
        }

    }

    else if (rtu->functionCode == 0x05)
    {
        printf("functionCode 값이 0x05일 경우 response 패킷을 구성합니다.\n\n");

        // response 배열(calloc으로 동적 할당)
        rtu->response = NULL;

        rtu->response = (byte*)calloc(8, sizeof(byte));

        // rtu 구조체 멤버 regStartAddr에 값 할당
        temp[0] = request[2];
        temp[1] = request[3];

        // bytesToShort 함수 사용
        rtu->regStartAddr = bytesToShort(temp);

        // memset 함수 사용
        memset(temp, 0, sizeof(temp));

        // rtu 구조체 멤버 a_ctrlValue에 값 할당
        temp[0] = request[4];
        temp[1] = request[5];

        rtu->a_ctrlValue = bytesToShort(temp);

        // memset 함수 사용
        memset(temp, 0, sizeof(temp));



        // RTU 구조체 response 배열에 값 할당
        // 정상적인 응답(response)은 질의문(request)의 되풀이된(Echo) 값이 오게 된다.

        rtu->response[0] = rtu->slaveAddr;
        rtu->response[1] = rtu->functionCode;


        rtu->response[2] = shortToBytes(rtu->regStartAddr)[0];
        rtu->response[3] = shortToBytes(rtu->regStartAddr)[1];

        rtu->response[4] = shortToBytes(rtu->a_ctrlValue)[0];
        rtu->response[5] = shortToBytes(rtu->a_ctrlValue)[1];


        if (rlen == 8)
        {
            // CRC 코드 값 할당
            rtu->crcValue[0] = request[6];
            rtu->crcValue[1] = request[7];

            rtu->response[6] = rtu->crcValue[0];
            rtu->response[7] = rtu->crcValue[1];


        }
        else
        {
            printf("request 패킷 구조가 잘못 되었습니다.\n패킷을 다시 구성하세요.\n");
            exit(1);

        }


    }

    else if (rtu->functionCode == 0x0F)
    {
        printf("functionCode 값이 0x0F일 경우 response 패킷을 구성합니다.\n\n");

        // response 배열(calloc으로 동적 할당)
        rtu->response = NULL;

        rtu->response = (byte*)calloc(9, sizeof(byte));

        // 레지스터 시작 주소

        temp[0] = request[2];
        temp[1] = request[3];

        // bytesToShort 함수 사용
        rtu->regStartAddr = bytesToShort(temp);

        // memset 함수 사용
        memset(temp, 0, sizeof(temp));

        // 제어 레지스터 수

        temp[0] = request[4];
        temp[1] = request[5];

        // bytesToShort 함수 사용
        rtu->ctrlRegCnt = bytesToShort(temp);

        // memset 함수 사용
        memset(temp, 0, sizeof(temp));

        // Data 바이트 수
        rtu->d_byteCnt = request[6];


        // RTU 구조체 response 배열에 값 할당
        // 정상적인 응답(response)은 질의문(request)의 되풀이된(Echo) 값이 오게 된다.
        rtu->response[0] = rtu->slaveAddr;
        rtu->response[1] = rtu->functionCode;

        // 레지스터 시작 주소
        rtu->response[2] = shortToBytes(rtu->regStartAddr)[0];
        rtu->response[3] = shortToBytes(rtu->regStartAddr)[1];

        // 제어 레지스터 수
        rtu->response[4] = shortToBytes(rtu->ctrlRegCnt)[0];
        rtu->response[5] = shortToBytes(rtu->ctrlRegCnt)[1];

        // Data 바이트 수
        rtu->response[6] = rtu->d_byteCnt;


        if (rlen == 10)
        {
            // CRC 코드 값 할당
            rtu->crcValue[0] = request[8];
            rtu->crcValue[1] = request[9];


            rtu->response[7] = rtu->crcValue[0];
            rtu->response[8] = rtu->crcValue[1];

        }
        else
        {
            printf("request 패킷 구조가 잘못 되었습니다.\n패킷을 다시 구성하세요.\n");
            exit(1);

        }


    }

    else if (rtu->functionCode == 0x10)
    {
        printf("functionCode 값이 0x10일 경우 response 패킷을 구성합니다.\n\n");

        // response 배열(calloc으로 동적 할당)
        rtu->response = NULL;

        rtu->response = (byte*)calloc(8, sizeof(byte));

        // 레지스터 시작주소
        temp[0] = request[2];
        temp[1] = request[3];

        rtu->regStartAddr = bytesToShort(temp);

        // memset 함수 사용
        memset(temp, 0, sizeof(temp));

        // 제어 레지스터 수
        temp[0] = request[4];
        temp[1] = request[5];

        rtu->ctrlRegCnt = bytesToShort(temp);

        // memset 함수 사용
        memset(temp, 0, sizeof(temp));


        // RTU 구조체 response 배열에 값 할당

        rtu->response[0] = rtu->slaveAddr;
        rtu->response[1] = rtu->functionCode;

        // 레지스터 시작 주소
        rtu->response[2] = shortToBytes(rtu->regStartAddr)[0];
        rtu->response[3] = shortToBytes(rtu->regStartAddr)[1];

        // 제어 레지스터 수
        rtu->response[4] = shortToBytes(rtu->ctrlRegCnt)[0];
        rtu->response[5] = shortToBytes(rtu->ctrlRegCnt)[1];


        if (rlen == 13)
        {
            // CRC 코드 값 할당
            rtu->crcValue[0] = request[11];
            rtu->crcValue[1] = request[12];


            rtu->response[6] = rtu->crcValue[0];
            rtu->response[7] = rtu->crcValue[1];


        }
        else
        {
            printf("request 패킷 구조가 잘못 되었습니다.\n패킷을 다시 구성하세요.\n");
            exit(1);
        }

    }

    else
    {
        printf("functionCode의 값을 잘못 입력 하였습니다.\n프로그램을 종료합니다.");
        exit(1);

    }

    return rtu->response;
}

// RTU request 패킷을 디코딩 해주는 함수 req_decoder
void req_decoder(byte * request, int req_len)
{
    // byte 배열 temp 선언 및 초기화
    byte temp[TWO_BYTES] = { 00, 00 };

    // RTU 구조체 생성 및 초기화
    RTU * rtu = NULL;

    rtu = MkRTU_init();

    // 1 단계 - 슬레이브 주소 값 할당
    rtu->slaveAddr = request[0];

    // 2 단계 - functionCode 값 할당
    rtu->functionCode = request[1];

    // 3 단계 - 레지스터 시작주소 값 할당
    temp[0] = request[2];
    temp[1] = request[3];

    // bytesToShort 함수 사용
    rtu->regStartAddr = bytesToShort(temp);

    // memset 함수 사용
    memset(temp, 0, sizeof(temp) / sizeof(byte));


    /* 16진수 출력 방법
       URL 주소
       https://dojang.io/mod/page/view.php?id=61 */
    printf("-----RTU Response 데이터 패킷 구조----- \n");
    printf("슬레이브 주소 : 0x%x\n\n", rtu->slaveAddr);
    printf("functionCode : 0x%x\n\n", rtu->functionCode);
    printf("레지스터 시작 주소 : 0x%x%x\n\n", shortToBytes(rtu->regStartAddr)[0],
           shortToBytes(rtu->regStartAddr)[1]);


    if (req_len == 8)
    {

        // 4 단계 - CRC 코드 값 할당
        rtu->crcValue[0] = request[6];
        rtu->crcValue[1] = request[7];



        if (rtu->functionCode == 0x01 || rtu->functionCode == 0x04)
        {
            // 5 단계 - 데이터 갯수 값 할당
            temp[0] = request[4];
            temp[1] = request[5];

            // bytesToShort 함수 사용
            rtu->dataCnt = bytesToShort(temp);

            // memset 함수 사용
            memset(temp, 0, sizeof(temp) / sizeof(byte));



            printf("데이터 갯수 : 0x%x%x\n\n", shortToBytes(rtu->dataCnt)[0],
                   shortToBytes(rtu->dataCnt)[1]);

            printf("CRC 코드 값 : 0x%x%x\n\n", rtu->crcValue[0], rtu->crcValue[1]);


        }
        else if (rtu->functionCode == 0x05)
        {
            // 5 단계 - 제어 상태값 할당
            temp[0] = request[4];
            temp[1] = request[5];

            // bytesToShort 함수 사용
            rtu->a_ctrlValue = bytesToShort(temp);

            // memset 함수 사용
            memset(temp, 0, sizeof(temp) / sizeof(byte));


            printf("제어 상태값 : 0x%x%x\n\n", shortToBytes(rtu->a_ctrlValue)[0],
                   shortToBytes(rtu->a_ctrlValue)[1]);

            printf("CRC 코드 값 : 0x%x%x\n\n", rtu->crcValue[0], rtu->crcValue[1]);


        }
        else
        {
            printf("request 패킷 구조가 잘못 되었습니다.\n패킷을 다시 구성하세요.\n");
            exit(1);

        }

    }
    else if (req_len == 10)
    {
        // 4 단계 - CRC 코드 값 할당
        rtu->crcValue[0] = request[8];
        rtu->crcValue[1] = request[9];

        if (rtu->functionCode == 0x0F)
        {
            // 5 단계 - 제어 레지스터 수 값 할당
            temp[0] = request[4];
            temp[1] = request[5];

            // bytesToShort 함수 사용
            rtu->ctrlRegCnt = bytesToShort(temp);

            // memset 함수 사용
            memset(temp, 0, sizeof(temp));

            // 6 단계 - Data 바이트 수 값 할당
            rtu->d_byteCnt = request[6];

            // 7 단계 - 제어 상태 값 할당
            rtu->ctrlValue = request[7];


            printf("제어 레지스터 수 : 0x%x%x\n\n", shortToBytes(rtu->ctrlRegCnt)[0], shortToBytes(rtu->ctrlRegCnt)[1]);
            printf("Data 바이트 수 : %x\n\n", rtu->d_byteCnt);
            printf("제어 상태 값 : 0x%x%x\n\n", shortToBytes(rtu->a_ctrlValue)[0], shortToBytes(rtu->a_ctrlValue)[1]);
            printf("CRC 코드 값 : 0x%x%x\n\n", rtu->crcValue[0], rtu->crcValue[1]);

        }

        else
        {
            printf("request 패킷 구조가 잘못 되었습니다.\n패킷을 다시 구성하세요.\n");
            exit(1);
        }

    }

    else if (req_len == 13)
    {
        // 4 단계 - CRC 코드 값 할당
        rtu->crcValue[0] = request[11];
        rtu->crcValue[1] = request[12];

        // 제어 Data 배열(calloc으로 동적 할당)
        rtu->ctrlData = (byte*)calloc(sizeof(byte)*(rtu->d_byteCnt), sizeof(byte));

        if (rtu->functionCode == 0x10)
        {
            // 5 단계 - 제어 레지스터 수 값 할당
            temp[0] = request[4];
            temp[1] = request[5];

            // bytesToShort 함수 사용
            rtu->ctrlRegCnt = bytesToShort(temp);

            // memset 함수 사용
            memset(temp, 0, sizeof(temp));

            // 6 단계 - Data 바이트 수 값 할당

            rtu->d_byteCnt = request[6];

            // 7 단계 - 제어 Data 배열 값 할당

            if (rtu->d_byteCnt == 04)
            {
                rtu->ctrlData[0] = request[7];
                rtu->ctrlData[1] = request[8];
                rtu->ctrlData[2] = request[9];
                rtu->ctrlData[3] = request[10];

            }
            else
            {
                printf("데이터 바이트 수를 잘못 입력 하였습니다.\n");
                exit(1);
            }

            printf("제어 레지스터 수 : 0x%x%x\n\n", shortToBytes(rtu->ctrlRegCnt)[0], shortToBytes(rtu->ctrlRegCnt)[1]);
            printf("데이터 바이트 수 : %x\n\n", rtu->d_byteCnt);
            printf("제어 Data 배열 : 0x%x%x%x%x\n\n", rtu->ctrlData[0], rtu->ctrlData[1], rtu->ctrlData[2], rtu->ctrlData[3]);
            printf("CRC 코드 값 : 0x%x%x\n\n", rtu->crcValue[0], rtu->crcValue[1]);

        }
        else
        {
            printf("request 패킷 구조가 잘못 되었습니다.\n패킷을 다시 구성하세요.\n");
            exit(1);
        }
    }

    else
    {
        printf("request 패킷 구조가 잘못 되었습니다.\n패킷을 다시 구성하세요.\n");
        exit(1);
    }

}

// RTU response 패킷을 디코딩 해주는 함수 res_decoder
void res_decoder(byte * response, int res_len)
{
    // byte형 배열 temp 선언 및 초기화
    byte temp[TWO_BYTES] = { 00, 00 };

    // RTU 구조체 생성 및 초기화
    RTU * rtu = NULL;

    rtu = MkRTU_init();

    //  1 단계 - 슬레이브 주소 값 할당
    rtu->slaveAddr = response[0];

    //  2 단계 - functionCode 값 할당
    rtu->functionCode = response[1];

    /* 16진수 출력 방법
        URL 주소
        https://dojang.io/mod/page/view.php?id=61 */
    printf("-----RTU Response 데이터 패킷 구조-----\n");
    printf("슬레이브 주소 : 0x%x\n\n", rtu->slaveAddr);
    printf("functionCode : 0x%x\n\n", rtu->functionCode);

    if (res_len == 8) // functionCode 가 0x01, 0x05, 0x10
    {
        // 5 단계 - CRC 코드 값 할당
        rtu->crcValue[0] = response[6];
        rtu->crcValue[1] = response[7];

        if (rtu->functionCode == 0x01)
        {
            // 3 단계 - 데이터 바이트 수 값 할당
            rtu->d_byteCnt = response[2];

            // 4 단계 - 제어 Data 배열 값 할당
            rtu->ctrlData = (byte*)calloc(sizeof(byte)*(rtu->d_byteCnt), sizeof(byte));

            rtu->ctrlData[0] = response[3];
            rtu->ctrlData[1] = response[4];
            rtu->ctrlData[2] = response[5];

            printf("데이터 바이트 수 : %x\n\n", rtu->d_byteCnt);
            printf("제어 Data 배열 : 0x%x%x%x\n\n", rtu->ctrlData[0], rtu->ctrlData[1], rtu->ctrlData[2]);
            printf("CRC 코드 값 : 0x%x%x\n\n", rtu->crcValue[0], rtu->crcValue[1]);
        }

        else if (rtu->functionCode == 0x05 || rtu->functionCode == 0x10)
        {
            // 3 단계 - 레지스터 시작주소 값 할당
            temp[0] = response[2];
            temp[1] = response[3];

            // bytesToShort 함수 사용
            rtu->regStartAddr = bytesToShort(temp);

            // memset 함수 사용
            memset(temp, 0, sizeof(temp) / sizeof(byte));

            if (rtu->functionCode == 0x05)
            {
                // 4 단계 - 제어 상태값 할당
                temp[0] = response[4];
                temp[1] = response[5];

                // bytesToShort 함수 사용
                rtu->a_ctrlValue = bytesToShort(temp);

                // memset 함수 사용
                memset(temp, 0, sizeof(temp) / sizeof(byte));

                printf("레지스터 시작 주소 : 0x%x%x\n\n", shortToBytes(rtu->regStartAddr)[0],
                       shortToBytes(rtu->regStartAddr)[1]);

                printf("제어 상태 값 : 0x%x%x\n\n", shortToBytes(rtu->a_ctrlValue)[0],
                       shortToBytes(rtu->a_ctrlValue)[1]);

                printf("CRC 코드 값 : 0x%x%x\n\n", rtu->crcValue[0], rtu->crcValue[1]);


            }
            else if (rtu->functionCode == 0x10)
            {
                // 4 단계 - 제어 레지스터 수 값 할당
                temp[0] = response[4];
                temp[1] = response[5];

                // bytesToShort 함수 사용
                rtu->ctrlRegCnt = bytesToShort(temp);

                // memset 함수 사용
                memset(temp, 0, sizeof(temp));

                printf("레지스터 시작 주소 : 0x%x%x\n\n", shortToBytes(rtu->regStartAddr)[0],
                       shortToBytes(rtu->regStartAddr)[1]);
                printf("제어 레지스터 수 : 0x%x%x\n\n", shortToBytes(rtu->ctrlRegCnt)[0],
                       shortToBytes(rtu->ctrlRegCnt)[1]);
                printf("CRC 코드 값 : 0x%x%x\n\n", rtu->crcValue[0], rtu->crcValue[1]);
            }

            else
            {
                printf("response 패킷 구조가 잘못 되었습니다.\n패킷을 다시 구성하세요.\n");
                exit(1);
            }

        }
        else
        {
            printf("response 패킷 구조가 잘못 되었습니다.\n패킷을 다시 구성하세요.\n");
            exit(1);
        }

    }

    else if (res_len == 9) // functionCode 가 0x04, 0x0F
    {
        // 제 5단계 - CRC 코드 값 할당
        rtu->crcValue[0] = response[7];
        rtu->crcValue[1] = response[8];

        if (rtu->functionCode == 0x04)
        {
            // 제 3 단계 - Data 바이트 수 값 할당
            rtu->d_byteCnt = response[2];

            // 제 4 단계 - 제어 Data 배열 값 할당
            rtu->ctrlData = (byte *)calloc(sizeof(byte) * (rtu->d_byteCnt), sizeof(byte));

            rtu->ctrlData[0] = response[3];
            rtu->ctrlData[1] = response[4];
            rtu->ctrlData[2] = response[5];
            rtu->ctrlData[3] = response[6];

            printf("데이터 바이트 수 : %x\n\n", rtu->d_byteCnt);
            printf("제어 데이터 배열 : 0x%x%x%x%x\n\n", rtu->ctrlData[0], rtu->ctrlData[1],
                   rtu->ctrlData[2], rtu->ctrlData[3]);
            printf("CRC 코드 값 : 0x%x%x\n\n", rtu->crcValue[0], rtu->crcValue[1]);
        }
        else if (rtu->functionCode == 0x0F)
        {
            // 제 3 단계 - 레지스터 시작 주소 값 할당
            temp[0] = response[2];
            temp[1] = response[3];

            // bytesToShort 함수 사용
            rtu->regStartAddr = bytesToShort(temp);

            // memset 함수 사용
            memset(temp, 0, sizeof(temp));

            // 제 4 단계 - 제어 레지스터 수 값 할당
            temp[0] = response[4];
            temp[1] = response[5];

            // bytesToShort 함수 사용
            rtu->ctrlRegCnt = bytesToShort(temp);

            // memset 함수 사용
            memset(temp, 0, sizeof(temp));

            // 제 6 단계 - Data 바이트 수 값 할당
            rtu->d_byteCnt = response[6];

            printf("레지스터 시작 주소 : 0x%x%x\n\n", shortToBytes(rtu->regStartAddr)[0],
                   shortToBytes(rtu->regStartAddr)[1]);
            printf("제어 레지스터 수 : 0x%x%x\n\n", shortToBytes(rtu->ctrlRegCnt)[0],
                   shortToBytes(rtu->ctrlRegCnt)[1]);

            printf("Data 바이트 수 : %x\n\n", rtu->d_byteCnt);
            printf("CRC 코드 값 : 0x%x%x\n\n", rtu->crcValue[0], rtu->crcValue[1]);


        }
        else
        {
            printf("response 패킷 구조가 잘못 되었습니다.\n패킷을 다시 구성하세요.\n");
            exit(1);
        }

    }
    else
    {
        printf("response 패킷 구조가 잘못 되었습니다.\n패킷을 다시 구성하세요.\n");
        exit(1);
    }

}

// RTU request 데이터 패킷(배열) 길이 체크해주는 함수 reqLenCheck
int reqLenCheck(byte * request)
{
    // 배열의 길이 저장할 변수
    int req_len = 0;
    // RTU 구조체 생성
    RTU * rtu = NULL;

    // RTU 구조체 초기화
    rtu = MkRTU_init();

    // 제 1단계 - functionCode 값 할당
    rtu->functionCode = request[1];

    if (rtu->functionCode == 0x01 || rtu->functionCode == 0x04 || rtu->functionCode == 0x05)
    {
        req_len = 8;
    }
    else if (rtu->functionCode == 0x0F)
    {
        req_len = 10;
    }
    else if (rtu->functionCode == 0x10)
    {
        req_len = 13;
    }

    return req_len;
}

// RTU response 데이터 패킷(배열) 길이 체크해주는 함수 resLenCheck
int resLenCheck(byte * response)
{
    // 배열의 길이 저장할 변수
    int res_len = 0;
    // RTU 구조체 생성
    RTU * rtu = NULL;

    // RTU 구조체 초기화
    rtu = MkRTU_init();

    // 제 1단계 - functionCode 값 할당
    rtu->functionCode = response[1];

    if (rtu->functionCode == 0x01 || rtu->functionCode == 0x05 || rtu->functionCode == 0x10)
    {
        res_len = 8;
    }
    else if (rtu->functionCode == 0x04 || rtu->functionCode == 0x0F)
    {
        res_len = 9;
    }


    return res_len;
}

// tcp response 패킷 디코딩 함수
void t_res_decoder(byte * response, short regStAddr)
{
    // TCP 구조체 생성 및 초기화
    TCP * tcp = NULL;

    tcp = MkTCP_init();

    // 1 단계 - transaction ID 값 할당
    tcp->tid[0] = response[0];
    tcp->tid[1] = response[1];

    // 2 단계 - pid 값 할당
    tcp->pid[0] = response[2];
    tcp->pid[1] = response[3];

    // 3 단계 - restLength 값 할당
    tcp->restLength[0] = response[4];
    tcp->restLength[1] = response[5];

    // 4 단계 - slaveID 값 할당
    tcp->slaveID = response[6];

    // 5 단계 - functionCode 값 할당
    tcp->functionCode = response[7];

    // 6 단계 - byteCnt 값 할당
    tcp->byteCnt = response[8];

    // 7 단계 - 출력하고자 하는 데이터 값 할당
    tcp->dataValue[0] = response[9];
    tcp->dataValue[1] = response[10];


    /* 16진수 출력 방법
       URL 주소
       https://dojang.io/mod/page/view.php?id=61 */
    printf("-----TCP Response 데이터 패킷 구조----- \n");
    printf("트랜잭션 아이디 : 0x%x%x\n\n", tcp->tid[0], tcp->tid[1]);
    printf("프로토콜 아이디 : 0x%x%x\n\n", tcp->pid[0], tcp->pid[1]);
    printf("Length 필드 이후로 남은 데이터 길이 : %u\n\n", tcp->restLength[0]);
    printf("슬레이브 주소 : 0x%x\n\n", tcp->slaveID);
    printf("functionCode : 0x%x\n\n", tcp->functionCode);
    printf("레지스터 갯수 : %u\n", tcp->byteCnt);

    if (shortToBytes(regStAddr)[0] == 0x01)
    {
        printf("CPU 사용량 : %u %\n\n", tcp->dataValue[0]);
    }
    else if (shortToBytes(regStAddr)[0] == 0x02)
    {
        printf("RAM 사용량 : %u MB\n\n", tcp->dataValue[0]);
    }
    else if (shortToBytes(regStAddr)[0] == 0x03)
    {
        printf("하드디스크 사용량 : %u MB\n\n", tcp->dataValue[0]);
    }
    else if (shortToBytes(regStAddr)[0] == 0x04)
    {
        printf("GPU 사용량 : %u %\n\n", tcp->dataValue[0]);
    }
    else
    {
        printf("레지스터 시작주소를 잘못 입력 하였습니다!!\n");
    }


}

// tcp request 패킷 디코딩 함수
void t_req_decoder(byte * request)
{
    // TCP 구조체 생성 및 초기화
    TCP * tcp = NULL;

    tcp = MkTCP_init();

    // 1 단계 - transaction ID 값 할당
    tcp->tid[0] = request[0];
    tcp->tid[1] = request[1];

    // 2 단계 - pid 값 할당
    tcp->pid[0] = request[2];
    tcp->pid[1] = request[3];

    // 3 단계 - restLength 값 할당
    tcp->restLength[0] = request[4];
    tcp->restLength[1] = request[5];

    // 4 단계 - slaveID 값 할당
    tcp->slaveID = request[6];

    // 5 단계 - functionCode 값 할당
    tcp->functionCode = request[7];


    // 6 단계 - regStAddr 값 할당
    tcp->regStartAddr[0] = request[8];
    tcp->regStartAddr[1] = request[9];

    // 7 단계 - regCnt 값 할당
    tcp->regCnt[0] = request[10];
    tcp->regCnt[1] = request[11];



    printf("-----TCP Request 데이터 패킷 구조----- \n");
    printf("트랜잭션 아이디 : 0x%x%x\n\n", tcp->tid[0], tcp->tid[1]);
    printf("프로토콜 아이디 : 0x%x%x\n\n", tcp->pid[0], tcp->pid[1]);
    printf("Length 필드 이후로 남은 데이터 길이 : %u\n\n", tcp->restLength[0]);
    printf("슬레이브 주소 : 0x%x\n\n", tcp->slaveID);
    printf("functionCode : 0x%x\n\n", tcp->functionCode);
    printf("레지스터 시작 주소 (상위바이트) : %x\n", tcp->regStartAddr[0]);
    printf("레지스터 시작 주소 (하위바이트) : %x\n\n", tcp->regStartAddr[1]);

    printf("레지스터 갯수 (상위바이트) : %x\n", tcp->regCnt[0]);
    printf("레지스터 갯수 (하위바이트) : %x\n\n", tcp->regCnt[1]);

}

// 데이퍼 패킷을 암호화 시켜주는 함수 encoding
int encoding(byte request[N])
{
    FILE * fp = fopen("encoding.txt", "wt");     // encoding 파일 생성

    int i = 0;     //   i = 출력 데이터
    int cnt = 1;   // cnt = 반복 횟수
    int idx = 0;   // idx = 입력 데이터

    byte results[N] = { 0, };   // 저장할 result 배열

    printf("인코딩 한 후 : ");
    for (i = 0; i < N; i++)   // 받아온 request[N]까지
    {
        if (request[i] != request[i + 1])      // 두 숫자가 다를 때
        {
            results[idx] = cnt;            // 첫 번째 자리에 인코딩 숫자인 cnt 넣기
            results[idx + 1] = request[i]; // 두 번째 자리에 카운트 대상 숫자 넣기
            cnt = 1;                       // 인코딩이 완료되면 cnt 숫자는 1로 초기화

            printf("%d%c", results[idx], results[idx + 1]);        // 두 자리씩 출력
            fprintf(fp, "%c%c", results[idx], results[idx + 1]);   // 아스키코드로 저장

            idx = idx + 2;   // 두 자리씩 끊기
        }
        else    // 두 숫자가 같다면
            cnt++;   // 인코딩 숫자인 cnt 증가

        if (request[i] == 0)      // request[N]이 끝날 경우 반복문 탈출
            break;
    }


    fclose(fp);     // 닫기
    printf("\n");


    return 1;
}

// 데이터 패킷을 암호화한 코드를 파일로 출력해주는 함수 decoding
void decoding(FILE * fp)
{
    char read[100];      // 불러올 문자열

    int i = 0;
    int j = 0;
    int n = 0;

    while (fgets(read, 30, fp))   // encoding 파일 내 문자열 읽기
        printf("저장된 데이터 : %s\n", read);

    printf("디코딩 : ");
    for (i = 0; i < 15; i++)      // 받아온 response[N] 까지
    {
        n = read[2 * i];         // 첫 번째 자리의 개수 n에 저장
        j = 0;                   // 카운터 값 초기화

        while (j < n)            // n개 까지
        {
            printf("%c", read[(2 * i) + 1]);   // 데이터를 n개만큼 출력(디코딩)
            j++;   // 카운터 값 증가

            if (read[(2 * i) + 1] == '\0')
                break;
        }

    }
}
