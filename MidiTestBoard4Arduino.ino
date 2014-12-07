/*
    MIDI信号を受け取り、3音同時に音を出す

    コンパイル環境　： Arduino IDE(Arduino 1.0.5)
    ターゲットボード： [Tools]→[Board]→[Da Vinci 32U]
    使用ライブラリ　：Arduino MIDI Library v4.2
                    ：Arduino Tone Library（custom）
                       #7 Tone.cpp use.
                       https://code.google.com/p/rogue-code/issues/detail?id=13

    開発ボード　　　：Strawberry Linux / Da Vinci 32U w/Arduino Bootloader/ATMEGA32U4

                                                @ichirowo
    2014/12/07    
    
*/

#include <Tone.h>
#include <MIDI.h>
 
//ピン設定
#define Speaker1 6
#define Speaker2 5
#define Speaker3 13


// MIDIからToneに変換-------------------------------------
int  m2t[127] = {
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
3,11,19,27,35,43,51,59,67,75,83,91,99,107,115,
123,    //NOTE_B2
131,    //NOTE_C3
139,    //NOTE_CS3
147,    //N 
196,    //NOTE_G3
208,    //NOTE_GS3
220,    //NOTE_A3
233,    //NOTE_AS3
247,    //NOTE_B3
262,    //NOTE_C4
277,    //NOTE_CS4
294,    //NOTE_D4
311,    //NOTE_DS4
330,    //NOTE_E4
349,    //NOTE_F4
370,    //NOTE_FS4
392,    //NOTE_G4
415,    //NOTE_GS4
440,    //NOTE_A4
466,    //NOTE_AS4
494,    //NOTE_B4
523,    //NOTE_C5
554,    //NOTE_CS5
587,    //NOTE_D5
622,    //NOTE_DS5
659,    //NOTE_E5
698,    //NOTE_F5
740,    //NOTE_FS5
784,    //NOTE_G5
831,    //NOTE_GS5
880,    //NOTE_A5
932,    //NOTE_AS5
988,    //NOTE_B5
1047,   //NOTE_C6
1109,   //NOTE_CS6
1175,   //NOTE_D6
1245,   //NOTE_DS6
1319,   //NOTE_E6
1397,   //NOTE_F6
1480,   //NOTE_FS6
1568,   //NOTE_G6
1661,   //NOTE_GS6
1760,   //NOTE_A6
1865,   //NOTE_AS6
1976,   //NOTE_B6
2093,   //NOTE_C7
2217,   //NOTE_CS7
2349,   //NOTE_D7
2489,   //NOTE_DS7
2637,   //NOTE_E7
2794,   //NOTE_F7
2960,   //NOTE_FS7
3136,   //NOTE_G7
3322,   //NOTE_GS7
3520,   //NOTE_A7
3729,   //NOTE_AS7
3951,   //NOTE_B7
4186,   //NOTE_C8
4435,   //NOTE_CS8
4699,   //NOTE_D8
4978    //NOTE_DS8
};

//変数初期化===========================================================
int tone1Interval = 0;
int tone2Interval = 0;
int tone3Interval = 0;

Tone tone1;
Tone tone2;
Tone tone3;

MIDI_CREATE_INSTANCE(HardwareSerial, Serial1, midi1);

void setup()
{
	tone1.begin(Speaker1);
	tone2.begin(Speaker2);
	tone3.begin(Speaker3);
	midi1.begin();
	Serial.begin(9600);
}
void loop()
{
	uint8_t code;
	uint8_t ch,data1,data2;
	ch = 1;		//Read channel set;
	if (midi1.read()) {
		Serial.println("read");
		midi1.setInputChannel(ch);  // Channel
 
		switch(midi1.getType()) {
			case midi::NoteOn:
				data1 = midi1.getData1();    // note no
				data2 = midi1.getData2();    // velocity
				if(data2 == 0){
					Serial.print("data2 = ");
					Serial.println(data2);
				} else if(tone1Interval == 0){
					tone1Interval = data1;
					tone1.play(m2t[data1]);
				} else if(tone2Interval == 0){
					tone2Interval = data1;
					tone2.play(m2t[data1]);
				} else if(tone3Interval == 0){
					tone3Interval = data1;
					tone3.play(m2t[data1]);
				}
			break;
			case midi::NoteOff:
				data1 = midi1.getData1();	// note no
				if(tone1Interval == data1){
					tone1Interval = 0;
					tone1.stop();
				}
				if(tone2Interval == data1){
					tone2Interval = 0;
					tone2.stop();
				}
				if(tone3Interval == data1){
					tone3Interval = 0;
					tone3.stop();
				}
			break;
			case midi::PitchBend:
				data2 = midi1.getData2();
				if(tone1Interval != 0){
					int tmp = tone1Interval + 19 + data2;
					tone1.play(tmp);
					Serial.print("tmp1 = ");
					Serial.println(tmp);
				}
				if(tone2Interval != 0){
					int tmp = tone2Interval + 19 + data2;
					tone2.play(tmp);
					Serial.print("tmp2 = ");
					Serial.println(tmp);
				}
				if(tone3Interval != 0){
					int tmp = tone3Interval + 19 + data2;
					tone3.play(tmp);
					Serial.print("tmp3 = ");
					Serial.println(tmp);
				}
			break;
			default:
				//
			break;
		}
	}

}