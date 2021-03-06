#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

#define FLAG "\x00\x01\x02\x03\x04\x05\x06\x07\x08\x09\x0a\x0b\x0c\x0d\x0e\x0f"

#define Nb 4
#define Nk 4
#define Nr 10

typedef uint8_t state_t[4][4];

const uint8_t sbox[256] = {0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76, 0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0,
                           0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15, 0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75,
                           0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84, 0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf,
                           0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8, 0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2,
                           0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73, 0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb,
                           0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79, 0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08,
                           0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a, 0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e,
                           0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf, 0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16
                          };

const uint8_t Rcon[16] = {0x01, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36, 0x6c, 0xd8, 0xab, 0x4d, 0x9a};

uint8_t xtime(uint8_t x) {
    return ((x << 1) ^ (((x >> 7) & 1) * 0x1b));
}

void AddRoundKey(uint8_t round, state_t state, uint8_t* RoundKey) {
    uint8_t i, j;
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            state[i][j] ^= RoundKey[(round * Nb * 4) + (i * Nb) + j];
        }
    }
}

void SubBytes(state_t state) {
    for (uint8_t i = 0; i < 4; i++) {
        for (uint8_t j = 0; j < 4; j++) {
            state[j][i] = sbox[state[j][i]];
        }
    }
}

void ShiftRows(state_t state) {
    uint8_t temp;

    temp        = state[0][1];
    state[0][1] = state[1][1];
    state[1][1] = state[2][1];
    state[2][1] = state[3][1];
    state[3][1] = temp;

    temp        = state[0][2];
    state[0][2] = state[2][2];
    state[2][2] = temp;

    temp        = state[1][2];
    state[1][2] = state[3][2];
    state[3][2] = temp;

    temp        = state[0][3];
    state[0][3] = state[3][3];
    state[3][3] = state[2][3];
    state[2][3] = state[1][3];
    state[1][3] = temp;
}

void MixColumns(state_t state) {
    uint8_t i;
    uint8_t Tmp, Tm, t;
    for (i = 0; i < 4; i++) {
        t   = state[i][0];
        Tmp = state[i][0] ^ state[i][1] ^ state[i][2] ^ state[i][3] ;
        Tm  = state[i][0] ^ state[i][1] ; Tm = xtime(Tm);  state[i][0] ^= Tm ^ Tmp ;
        Tm  = state[i][1] ^ state[i][2] ; Tm = xtime(Tm);  state[i][1] ^= Tm ^ Tmp ;
        Tm  = state[i][2] ^ state[i][3] ; Tm = xtime(Tm);  state[i][2] ^= Tm ^ Tmp ;
        Tm  = state[i][3] ^ t ;           Tm = xtime(Tm);  state[i][3] ^= Tm ^ Tmp ;
    }
}

void KeyExpansion(uint8_t* RoundKey, uint8_t* Key) {
    uint8_t tempa[4];

    for (int i = 0; i < Nk; i++) {
        RoundKey[(i * 4) + 0] = Key[(i * 4) + 0];
        RoundKey[(i * 4) + 1] = Key[(i * 4) + 1];
        RoundKey[(i * 4) + 2] = Key[(i * 4) + 2];
        RoundKey[(i * 4) + 3] = Key[(i * 4) + 3];
    }

    for (int i = Nk; i < Nb * (Nr + 1); i++) {
        int j = (i - 1) * 4;

        tempa[0] = RoundKey[j + 0];
        tempa[1] = RoundKey[j + 1];
        tempa[2] = RoundKey[j + 2];
        tempa[3] = RoundKey[j + 3];

        if (i % Nk == 0) {
            j = tempa[0];
            tempa[0] = tempa[1];
            tempa[1] = tempa[2];
            tempa[2] = tempa[3];
            tempa[3] = j;

            tempa[0] = sbox[tempa[0]];
            tempa[1] = sbox[tempa[1]];
            tempa[2] = sbox[tempa[2]];
            tempa[3] = sbox[tempa[3]];

            tempa[0] = tempa[0] ^ Rcon[i / Nk];
        }

        int k = i * 4; j = (i - Nk) * 4;
        RoundKey[k + 0] = RoundKey[j + 0] ^ tempa[0];
        RoundKey[k + 1] = RoundKey[j + 1] ^ tempa[1];
        RoundKey[k + 2] = RoundKey[j + 2] ^ tempa[2];
        RoundKey[k + 3] = RoundKey[j + 3] ^ tempa[3];
    }
}

void GetRandom(uint8_t* buf, int len) {
    int fd = open("/dev/urandom", O_RDONLY);
    if (fd < 0) exit(1);

    int r = read(fd, buf, len);
    if (r < 0) exit(1);
    if (r != len) exit(1);

    close(fd);
}

void DumpState(state_t state) {
    for (uint8_t i = 0; i < 4; i++) {
        for (uint8_t j = 0; j < 4; j++) {
            printf("%02x ", state[j][i]);
        }
        printf("\n");
    }
    printf("\n");
    for (uint8_t i = 0; i < 4; i++) {
        for (uint8_t j = 0; j < 4; j++) {
            printf("%02x", state[i][j]);
        }
    }
    printf("\n");
}

void InjectFault(state_t state, uint8_t fault_pos) {
    uint8_t fault_val = 0;

    while(fault_val == 0){
        GetRandom(&fault_val, 1);
    }

    state[fault_pos & 3][(fault_pos >> 2) & 3] ^= fault_val;
}

void Encrypt(state_t state, uint8_t* RoundKey) {
    uint8_t round = 0;

    AddRoundKey(round, state, RoundKey);

    for (round = 1; round < 10; round++) {
        SubBytes(state);
        ShiftRows(state);
        MixColumns(state);
        AddRoundKey(round, state, RoundKey);
        if (round == 8 || round == 9) {
            printf("round %d:\n", round);
            DumpState(state);
        }
    }

    SubBytes(state);
    ShiftRows(state);
    printf("round %d:\n", round);
    DumpState(state);
    printf("last round key:\n");
    DumpState(RoundKey + round * Nb * 4);
    AddRoundKey(round, state, RoundKey);
}

void EncryptWithFault(state_t state, uint8_t* RoundKey) {
    uint8_t round = 0;

    AddRoundKey(round, state, RoundKey);

    for (round = 1; round < 8; round++) {
        SubBytes(state);
        ShiftRows(state);
        MixColumns(state);
        AddRoundKey(round, state, RoundKey);
    }

    uint8_t fault_pos = 0;
    // GetRandom(&fault_pos, 1);

    for (round = 8; round < 10; round++) {
        InjectFault(state, fault_pos);
        SubBytes(state);
        ShiftRows(state);
        MixColumns(state);
        AddRoundKey(round, state, RoundKey);
        printf("round %d:\n", round);
        DumpState(state);
    }

    InjectFault(state, fault_pos);
    SubBytes(state);
    ShiftRows(state);
    printf("round %d:\n", round);
    DumpState(state);
    printf("last round key:\n");
    DumpState(RoundKey + round * Nb * 4);
    AddRoundKey(round, state, RoundKey);
}

void CopyState(state_t dst, state_t src) {
    memcpy(dst, src, 16);
}

int main(int argc, char** argv) {
    uint8_t key[16];
    uint8_t RoundKey[176];
    state_t pt = {0};
    state_t state;

    memcpy(key, FLAG, 16);
    GetRandom((uint8_t *)pt, 16);
    KeyExpansion(RoundKey, key);

    CopyState(state, pt);
    Encrypt(state, RoundKey);
    DumpState(state);

    CopyState(state, pt);
    EncryptWithFault(state, RoundKey);
    DumpState(state);
}

