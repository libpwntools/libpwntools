#include <iostream>
#include <string>
#include <vector>
#include <string.h>
#include <stdint.h>
#include <unordered_map>
#include <stdio.h>
#include <cstddef>
#include <list>

#define AMD64 64
#define I386 86

struct packed_64 {
		char bytes[8];
}__attribute__((packed));

struct packed_32 {
		char bytes[4];
}__attribute__((packed));

class Char {
 public:
		char *bytes;
		uint64_t len;

		Char(uint64_t l) {
		 this->len = l;
			this->bytes = new char[len + 1];
			if(!this->bytes) {
					std::cout << "(New): Exiting" << std::endl;
					exit(-1);
			}
		}
		~Char() { delete this->bytes; }
		char* raw() { return this->bytes; }

		char at(uint32_t index) {
		 if(index >= this->len || index < 0) {
				std::cout << "(at): Invalid Index" << std::endl;
				exit(-1);
		 }
			return this->bytes[index];
		}

		void put(FILE *fp) { fwrite(this->bytes, this->len, 1, fp); }

		void ljust(uint64_t size, const char fillcharacter) {
			if (size <= this->len ) {
				std::cout << "(ljust): size < base_size." << std::endl;
				exit(-1);
			}
			uint64_t new_size = size + this->len;
			char* temp_bytes = new char[new_size + 1];
			for(int i = 0; i < this->len; i++) {temp_bytes[i] = this->bytes[i]; }
			for(int i = this->len ; i < size; i++) { temp_bytes[i] = fillcharacter; }
			delete this->bytes;
			this->len = new_size;
			this->bytes = temp_bytes;	
		}

		Char* sustitute(uint32_t s, uint32_t t) {
			if(s < 0 || s >= t || s >= this->len || t <= 0 || t >= this->len) {
				std::cout << "(Substitute): " << std::endl;
				exit(-1);
			}
			uint64_t required_len = t - s;
			Char* c = Char(required_len);
			for(int i = t, k = 0 ; i < s; i ++ ) { c[k++] = this->bytes[i]; }
			return c;
		}

};

class General {
	public:
	 Char* Add(std::vector<Char*> arguments) {
			uint64_t total_size_required {0};
			int i,k;
			for(i = 0; i < arguments.size(); i++) { total_size_required += arguments.at(i)->len; }
			Char* c = new Char(total_size_required);
			for(i = 0,k = 0; i < arguments.size(); i++ ) {
				for (int j = 0; j < arguments.at(i)->len; j++ ) { c->bytes[k++] = arguments.at(i)->bytes[j]; }
			}
			return c;
		}
};

class Struct {
 public:	
		/* Pack 64 bit number to bytes */
		packed_64 pack64(int64_t number) {
			packed_64 packed_bytes;
			for(int i = 0; i < sizeof(packed_bytes); i++) {
				packed_bytes.bytes[i] = number & 0xff;
				number >>= 8;
			}
			return packed_bytes;
		}
		/* Pack 32 bit number to char bytes */
		packed_32 pack32(int32_t number) {
		 packed_32 packed_bytes;
			for(int i = 0; i < sizeof(packed_bytes); i++){
				packed_bytes.bytes[i] = number & 0xff;
				number >>= 8;
			}
			return packed_bytes;
		}
		/* Unpack bytes to 64 bit number */
		int64_t unpack64(packed_64 packed_bytes) { 
			int64_t unpacked_number {0};
			for(int i = 1; i < 9 ; i++) {
				unpacked_number += ( ( (int64_t)packed_bytes.bytes[i - 1] ) << ( 64 - (8 * i) ) );
   }
			return unpacked_number;
		}
		/* Unpack bytes to 32 bit number */
		int32_t unpack32(packed_32 packed_bytes) {
			int32_t unpacked_number {0};
			for(int i = 1; i < 5 ; i++) {
				unpacked_number += ( ( (int32_t)packed_bytes.bytes[i - 1] ) << ( 32 - (8 * i) )  );
			}
			return unpacked_number;
		}
		/* Flat same as pwntools flat */
		Char* flat(std::vector<int64_t> chain) {
				Char *c = new Char(chain.size() * 8);
				packed_64 temp;
				for(int i = 0, k = 0; i < chain.size(); i++) {
					temp = this->pack64(chain.at(i));
					for(int j = 0; j < 8; j++) { c->bytes[k++] = temp.bytes[j]; }
				}
				return c;
		}
		Char* flat(std::vector<int32_t> chain) {
			Char* c = new Char(chain.size() * 4 );
			packed_32 temp;
			for(int o = 0, p = 0; o < chain.size(); o++) {
				temp = this->pack32(chain.at(o));
				for(int l = 0; l < 4; l++) {c->bytes[p++] = temp.bytes[l];}
			}
			return c;
		}
};

class Remote {

 public:
		const char* host_ip;
		uint32_t port;

		Remote(const char* ip,uint32_t p) {
		 this->host_ip = ip;
			this->port = p;
		}


};
