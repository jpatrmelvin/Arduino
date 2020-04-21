#include "SerialRX.h"


// itteritively calculate checksum of a message. 
// PARAM chk: 0 if first checksum use, preious return if else.
unsigned char SerialRX::calculate_BSD_single(unsigned char chk, unsigned char data) {
    unsigned char ret = (chk >> 1) + (chk << 7);
    ret += data;
    return ret;
}



void SerialRX::Init(void){
	state = WAIT_FOR_HEAD;
	head = 0;
	tail = 0;
}

int SerialRX::process(unsigned char data){
	// keeps track of number of chars we have received in payload
	static unsigned char payload_idx;
	// holds current chacksum value.
	static unsigned char checksum;
	
	switch(state){
		case WAIT_FOR_HEAD:
			if(data == HEAD){
				state = READ_ID;
			}
			break;
		case READ_ID:
			message_id[tail] = data;
			state = READ_LENGTH;
			break;
		case READ_LENGTH:
			message_len[tail] = data;
			state = READ_PAYLOAD;
			checksum = 0;
			payload_idx = 0;
			break;
		case READ_PAYLOAD:
			message_data[tail][payload_idx] = data;
			checksum = calculate_BSD_single(checksum, data);
			payload_idx++;
			if(payload_idx >= message_len[tail]){
				state = READ_TAIL;
			}
			break;
		case READ_TAIL:
			if(data == TAIL){
				state = READ_CHECKSUM;
			} else {
				state = WAIT_FOR_HEAD;
				return FAILURE;
			}
			break;
		case READ_CHECKSUM:
			state = WAIT_FOR_HEAD;
			if(data == checksum){
				tail = (tail + 1) % MAX_MESSAGES;
			} else {
				return checksum;
			}
			
			break;
	}
	return SUCCESS;
}

bool SerialRX::available(void){
	if(head != tail){
		return TRUE;
	}
	return FALSE;
}


int SerialRX::get_length(void){
	if(this->available()){
		return (int) message_len[head];
	}
	return 0;
}



int SerialRX::get_data(int len, unsigned char data[]){
	if(this->available()){
		for(int i = 0; i < len; i++){
			data[i] = message_data[head][i];
		}
		head = (head + 1) % MAX_MESSAGES;
		return SUCCESS;
	}
	return FAILURE;
}


unsigned char SerialRX::get_ID(void){
	if(this->available()){
		return message_id[head];
	}
	return FAILURE;
}

