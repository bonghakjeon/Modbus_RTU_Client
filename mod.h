//
// Created by wjsqh on 2020-07-19.
//

#ifndef MOD_MASTER_MOD_H
#define MOD_MASTER_MOD_H

#define TO_BYTES_LEN 12
#define MBHEADER_LEN 7
#define TWO_BYTES 2
#define RTU_LEN 14
#define RESPONSE_LEN 13

// 데이터 패킷 암호화, 복호화 관련 encoding, decoding 함수 매크로 상수
#define ESC '*'
#define N 256*256

// UnitId(1 byte) + Function Code(1 byte) + Start Address(2 byte)
#define Unit_FC_StAddr 0x04

typedef unsigned char byte;   // 자료형의 크기 1 byte

// MBAP Header에 있는 Transaction ID(short형으로 형변환 해야함.)
unsigned char transactionId = 0x00;

byte toBytes[TO_BYTES_LEN]; // encoder, decoder 함수에 사용하기 위해 전역 변수(배열)로 선언한 배열 toBytes(배열의 길이 12)

byte response[RESPONSE_LEN];

enum {
    // 디지털 출력포트 상태 확인
    READ_COIL = 0x01,

    // 디지털 입력포트 상태 확인
    READ_DISCRETE_INPUTS = 0x02,

    // 디지털/아날로그 입력포트, 디지털 출력포트 상태 확인
    READ_HOLDING_REGISTERS = 0x03,

    // 디지털/아날로그 입력포트 상태 확인
    READ_INPUT_REGISTER = 0x04,

    // 하나의 디지털 출력포트 ON/OFF 제어에 사용
    WRITE_SINGLE_COIL = 0x05,

    // 하나의 출력포트 ON/OFF 제어에 사용
    WRITE_SINGLE_REGISTER = 0x06,

    // 연속적으로 출력포트 ON/OFF 제어에 사용
    WRITE_MULTIPLE_REGISTERS = 0x10,

    // 연속적으로 있는 여러 개의 디지털 출력 포트 ON/OFF 제어에 사용
    WRITE_MULLTIPLE_COILS = 0x0F
};

// MBAPHeader 구조체의 정의
typedef struct
{
    byte header[MBHEADER_LEN];
} MBAPHeader;

// Modbus TCP 관련 AppDataUnit 구조체의 정의
typedef struct
{
    // ADU를 구성하고 있는 요소들을 멤버로 구성
    MBAPHeader mbHeader;

    byte functionCode;

    byte startAddr[TWO_BYTES];
    // unsigned char 배열 startAddr(시작주소) 상위바이트 하위바이트로 나눠서 비트 값으로 저장된다.

    byte a_dataCnt[TWO_BYTES];
    // unsigned char 배열 a_dataCnt(시작 주소 번지부터 읽어올 데이터의 갯수)
    /* (예를들어 int형 값이 들어오면 정수로 값이 저장 되는게 아니라, 상위바이트 하위바이트로 나눠서 비트 값으로
        저장된다.)(shortToBytes 함수 참고!!!) */
} AppDataUnit;


typedef struct
{
    // transaction ID
    byte tid[TWO_BYTES];

    // pid
    byte pid[TWO_BYTES];

    // restLength
    byte restLength[TWO_BYTES];

    // slaveID
    byte slaveID;

    // functionCode
    byte functionCode;

    // 레지스터 시작 주소
    byte regStartAddr[TWO_BYTES];

    // 레지스터 수
    byte regCnt[TWO_BYTES];

    // 바이트 수
    byte byteCnt;

    // 데이터 값
    byte dataValue[TWO_BYTES];

    // request 배열(calloc으로 동적 할당)
    byte * request;

    // response 배열(calloc으로 동적 할당)
    byte * response;

} TCP;


typedef struct
{
    // request & response 공용 멤버 변수

    // Data 바이트 수 (request & response 공용)(byte 자료형의 갯수)
    byte d_byteCnt;

    // request 관련 멤버 변수

    // 슬레이브 주소 (request & response 공용)
    byte slaveAddr;

    // functionCode (request & response 공용)
    byte functionCode;

    // 레지스터 시작 주소 (request & response 공용)
    short regStartAddr;

    // 데이터 갯수
    short dataCnt;

    // 제어 상태 값 배열(request & response 공용)
    short a_ctrlValue;

    // 제어 상태 값
    byte ctrlValue;

    // CRC 코드 값
    byte crcValue[TWO_BYTES];

    // 제어 레지스터 수 (request & response 공용)
    short ctrlRegCnt;

    // 제어 Data 배열(calloc으로 동적 할당)
    byte * ctrlData;

    // request 배열(calloc으로 동적 할당)
    byte * request;


    // response 관련 멤버 변수

    // Data 배열(calloc으로 동적 할당)
    byte * arr_data;

    // response 배열(calloc으로 동적 할당)
    byte * response;

} RTU;



// 빅엔디안 함수 shortToByte 함수에 사용할 구조체 DBByte_t의 정의
typedef struct dbbyte_s
{
    byte upper;    // 상위 바이트
    byte lower;    // 하위 바이트
} DBByte_t;

// 빅엔디안 함수 shortToByte 함수에 사용할 공용체 STBBuf_t의 정의
typedef union stbbuf
{
    short iBuf;
    byte bBuf[2];
    DBByte_t stbBuf;
} STBBuf_t;

// crc16_ShortToBytes 함수에 사용할 공용체 ErrCheck_t의 정의
typedef union errcheck
{
    int icrc;
    byte b_num[2];
} ErrCheck_t;


// CRC 에러코드 값들을 배열의 각 요소에 할당함.
const unsigned short CRC_Table[] =
        {
                0X0000, 0XC0C1, 0XC181, 0X0140, 0XC301, 0X03C0, 0X0280, 0XC241, 0XC601, 0X06C0,
                0X0780, 0XC741, 0X0500, 0XC5C1, 0XC481, 0X0440, 0XCC01, 0X0CC0, 0X0D80, 0XCD41,
                0X0F00, 0XCFC1, 0XCE81, 0X0E40, 0X0A00, 0XCAC1, 0XCB81, 0X0B40, 0XC901, 0X09C0,
                0X0880, 0XC841, 0XD801, 0X18C0, 0X1980, 0XD941, 0X1B00, 0XDBC1, 0XDA81, 0X1A40,
                0X1E00, 0XDEC1, 0XDF81, 0X1F40, 0XDD01, 0X1DC0, 0X1C80, 0XDC41, 0X1400, 0XD4C1,
                0XD581, 0X1540, 0XD701, 0X17C0, 0X1680, 0XD641, 0XD201, 0X12C0, 0X1380, 0XD341,
                0X1100, 0XD1C1, 0XD081, 0X1040, 0XF001, 0X30C0, 0X3180, 0XF141, 0X3300, 0XF3C1,
                0XF281, 0X3240, 0X3600, 0XF6C1, 0XF781, 0X3740, 0XF501, 0X35C0, 0X3480, 0XF441,
                0X3C00, 0XFCC1, 0XFD81, 0X3D40, 0XFF01, 0X3FC0, 0X3E80, 0XFE41, 0XFA01, 0X3AC0,
                0X3B80, 0XFB41, 0X3900, 0XF9C1, 0XF881, 0X3840, 0X2800, 0XE8C1, 0XE981, 0X2940,
                0XEB01, 0X2BC0, 0X2A80, 0XEA41, 0XEE01, 0X2EC0, 0X2F80, 0XEF41, 0X2D00, 0XEDC1,
                0XEC81, 0X2C40, 0XE401, 0X24C0, 0X2580, 0XE541, 0X2700, 0XE7C1, 0XE681, 0X2640,
                0X2200, 0XE2C1, 0XE381, 0X2340, 0XE101, 0X21C0, 0X2080, 0XE041, 0XA001, 0X60C0,
                0X6180, 0XA141, 0X6300, 0XA3C1, 0XA281, 0X6240, 0X6600, 0XA6C1, 0XA781, 0X6740,
                0XA501, 0X65C0, 0X6480, 0XA441, 0X6C00, 0XACC1, 0XAD81, 0X6D40, 0XAF01, 0X6FC0,
                0X6E80, 0XAE41, 0XAA01, 0X6AC0, 0X6B80, 0XAB41, 0X6900, 0XA9C1, 0XA881, 0X6840,
                0X7800, 0XB8C1, 0XB981, 0X7940, 0XBB01, 0X7BC0, 0X7A80, 0XBA41, 0XBE01, 0X7EC0,
                0X7F80, 0XBF41, 0X7D00, 0XBDC1, 0XBC81, 0X7C40, 0XB401, 0X74C0, 0X7580, 0XB541,
                0X7700, 0XB7C1, 0XB681, 0X7640, 0X7200, 0XB2C1, 0XB381, 0X7340, 0XB101, 0X71C0,
                0X7080, 0XB041, 0X5000, 0X90C1, 0X9181, 0X5140, 0X9301, 0X53C0, 0X5280, 0X9241,
                0X9601, 0X56C0, 0X5780, 0X9741, 0X5500, 0X95C1, 0X9481, 0X5440, 0X9C01, 0X5CC0,
                0X5D80, 0X9D41, 0X5F00, 0X9FC1, 0X9E81, 0X5E40, 0X5A00, 0X9AC1, 0X9B81, 0X5B40,
                0X9901, 0X59C0, 0X5880, 0X9841, 0X8801, 0X48C0, 0X4980, 0X8941, 0X4B00, 0X8BC1,
                0X8A81, 0X4A40, 0X4E00, 0X8EC1, 0X8F81, 0X4F40, 0X8D01, 0X4DC0, 0X4C80, 0X8C41,
                0X4400, 0X84C1, 0X8581, 0X4540, 0X8701, 0X47C0, 0X4680, 0X8641, 0X8201, 0X42C0,
                0X4380, 0X8341, 0X4100, 0X81C1, 0X8081, 0X4040
        };

// -----------------------------------------------------------------------------------------------------------

// MBAPHeader 관련 함수 4개 + 1개

// MBAPHeader 생성 및 초기화 시켜주는 함수 MkMbHeader_init
MBAPHeader* MkMbHeader_init(void);

// RTU 생성 및 초기화 시켜주는 함수 MkRTU_init
RTU* MkRTU_init(void);

// TCP 생성 및 초기화 시켜주는 함수 MkTCP_init
TCP* MkTCP_init(void);

// MBAPHeader를 byte 배열 형태로 반환해주는 함수 getHeader
byte* getHeader(MBAPHeader * mbHeader);

// 인자로 전달된 a_header를 구조체 MBAPHeader의 멤버변수 header에 저장하는 함수 setMbHeader
void setMbHeader(MBAPHeader * mbHeader, byte * a_header);

// 인자로 전달된 mbHeader에 저장된 내용(배열)을 출력 해주는 함수 ShowInfoMbHeader
void ShowInfoMbHeader(MBAPHeader * a_mbHeader);



// -----------------------------------------------------------------------------------------------------------


// AppDataUnit 관련 함수 11개 + 1개

// AppDataUnit 생성 및 초기화 시켜주는 함수 MkAppDataUnit_init
AppDataUnit* MkAppDataUnit_init(void);


// AppDataUnit 구조체 변수의 멤버변수들을 인자로 받아서 구조체 AppDataUnit의 멤버에 값을 저장해 준 후 ADU 구조체 변수를 반환하는 함수 AppDataUnit_set
AppDataUnit* AppDataUnit_set(MBAPHeader * mbapHeader, byte a_funcCode, byte * stAddr, byte * len);

// 슬레이브에서 응답받은 byte 배열 response를 인자로 받아서 ADU 구조체 변수에 저장해준 후에 ADU 구조체 변수를 반환하는 함수 AppDataUnit_Slave
AppDataUnit* AppDataUnit_Slave(byte * response);

// AppDataUnit 구조체 변수를 인자로 받아서 MBAPHeader로 반환해주는 함수 getMbHeader
MBAPHeader* getMbHeader(AppDataUnit * adu);

// byte 배열 a_header를 인자로 받아서 구조체 AppDataUnit의 멤버 변수인 mbapHeader에 값을 저장해 주는 함수 setMbHeader
void setADUHeader(AppDataUnit * adu, byte * a_header);

// AppDataUnit 구조체 변수를 인자로 받아서 구조체의 멤버인 functionCode에 저장된 값을 반환해주는 함수 getFunctionCode
byte getFunctionCode(AppDataUnit * adu);

// AppDataUnit 구조체 변수와 a_FuncCode를 인자로 받아서 구조체 AppDataUnit의 멤버 변수인 functionCode에 값을 저장해 주는 함수 setFunctionCode
void setFunctionCode(AppDataUnit * adu, byte a_funcCode);

/* AppDataUnit 구조체 변수를 인자로 받아서 구조체의 멤버인 바이트 배열 startAddr에 저장된 값을 byte 배열 형태로 반환해주는 함수 getStartAddr */
byte* getStartAddr(AppDataUnit * adu);

/* byte 배열 startAddress를 인자로 받아서 구조체 AppDataUnit의 멤버인 바이트 배열 startAddr에 값을 저장해 주는 함수 setStartAddr */
void setStartAddr(AppDataUnit * adu, byte * startAddress);

/* AppDataUnit 구조체 변수를 인자로 받아서 구조체 AppDataUnit의 멤버인 바이트 배열 length에 저장된 값을
   byte 배열 형태로 반환해주는 함수(상위바이트/ 하위바이트로 나눠서 길이값을 비트 값으로 표현함.) */
byte* getLength(AppDataUnit * adu);

/* byte 배열 len을 인자로 받아서 구조체 AppDataUnit의 멤버인 바이트 배열 length에 값을 저장해 주는 함수 setLength */
void setLength(AppDataUnit * adu, byte * len);

/* AppDataUnit 구조체 변수를 인자로 받아서 구조체 AppDataUnit에 저장된 내용을 출력해주는 함수 ShowInfoAppDataUnit */
void ShowInfoAppDataUnit(AppDataUnit * adu);


// ----------------------------------------------------------------------------------------------------------


// Modbus 관련 함수 5개
// AppDataUnit 구조체형 변수 형태로 요청할(Parameter) 값을 셋팅해 주는 함수 setAdu
AppDataUnit* setAdu(byte * a_header, byte a_funcCode, short stAddr, byte * len);

// byte 배열을 인자로 받아 구조체 변수 MBAPHeader 값을 셋팅해주는 함수 setHeader 반환할 때는 MBAPHeader 구조체로 반환한다.
MBAPHeader* setHeader(byte * a_header);


// 마스터에서 Request한 값을 슬레이브에서 byte 배열 형태로 변환해주는 함수 encoder
byte* encoder(AppDataUnit * adu);

// Response 받은 값을 byte 배열로 반환해주는 함수 decoder
byte* decoder(byte* response);

// short 형태로 받은 자료형을 byte 배열 형태로 변환해주는 함수 shortToBytes
byte* shortToBytes(short value);

/* byte 형태로 받은 배열을 short형 자료형 변수로 변환 해주는 함수 bytesToShort */
short bytesToShort(byte * param);

// CRC16 을 byte 배열로 로 반환해주는 함수 crc16_ShortToBytes
byte* crc16_ShortToBytes(byte * a_crc16, int ilen);

// main 함수에서 Response를 출력하는 함수 ShowInfo_Res
void ShowInfo_Res(AppDataUnit * res_adu);

// RTU request 데이터 패킷으로 셋팅해주는 함수 rtuSetRequest
byte* rtuSetRequest(byte slvID, byte funcCode, short regStAddr, byte * a_crc16, unsigned int crc_len);

// RTU response 데이터 패킷으로 셋팅해주는 함수 rtuSetResponse
byte* rtuSetResponse(byte * request, int rlen);

// RTU request 패킷을 디코딩 해주는 함수 req_decoder
void req_decoder(byte * request, int req_len);

// RTU response 패킷을 디코딩 해주는 함수 res_decoder
void res_decoder(byte * response, int res_len);

// RTU request 데이터 패킷(배열) 길이 체크해주는 함수 reqLenCheck
int reqLenCheck(byte * request);

// RTU response 데이터 패킷(배열) 길이 체크해주는 함수 resLenCheck
int resLenCheck(byte * response);

// TCP response 패킷을 디코딩 해주는 함수 t_res_decoder
void t_res_decoder(byte * response, short regStAddr);

// TCP request 패킷을 디코딩 해주는 함수 t_req_decoder
void t_req_decoder(byte * request);

// 데이퍼 패킷을 암호화 시켜주는 함수 encoding
int encoding(byte request[N]);

// 데이터 패킷을 암호화한 코드를 파일로 출력해주는 함수 decoding
void decoding(FILE * fp);






#endif //MOD_MASTER_MOD_H
