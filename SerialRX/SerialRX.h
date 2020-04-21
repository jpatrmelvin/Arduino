// Class to receive and process data coming from a serial input_iterator
// Written by James Melvin
// Date: 12/22/2019



#define MAX_MESSAGES 3
#define MAX_MESSAGE_LEN 255

#define TRUE 1
#define FALSE 0
#define SUCCESS 1
#define FAILURE -1

// Data to be reeived needs to be in format: [HEAD -- ID -- LEN -- PAYLOAD -- END -- CHECKSUM]
class SerialRX{
	public:
	
		// init receiver
		void Init(void);
		// Process incoming char data
		int process(unsigned char data);
		// check if valid message is available. return FALSE if no message, TRUE if valid message received.
		bool available(void);
		// returns the length of next message in queue
		int get_length(void);
		// returns next ID.+
		unsigned char get_ID(void);
		// fills target array with data
		int get_data(int len, unsigned char data[]);

		
	private:
	// fcn to calculate checksum of payload data
		unsigned char calculate_BSD_single(unsigned char chk, unsigned char data);
		
		typedef enum {
			WAIT_FOR_HEAD = 0,
			READ_ID,
			READ_LENGTH,
			READ_PAYLOAD,
			READ_TAIL,
			READ_CHECKSUM
		} States;
		
		States state;
		unsigned char checksum;
		
		// packet storage structures
		unsigned int head, tail;
		unsigned char message_id[MAX_MESSAGES];
		unsigned char message_len[MAX_MESSAGES];
		unsigned char message_data[MAX_MESSAGES][MAX_MESSAGE_LEN];
		
		
		
		
};
		
		