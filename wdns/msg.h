#ifndef WDNS_MSG_H
#define WDNS_MSG_H

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

/* Macros. */

#define WDNS_FLAGS_QR(msg)		((((msg).flags) >> 15) & 0x01)
#define WDNS_FLAGS_OPCODE(msg)		((((msg).flags) >> 11) & 0x0f)
#define WDNS_FLAGS_AA(msg)		((((msg).flags) >> 10) & 0x01)
#define WDNS_FLAGS_TC(msg)		((((msg).flags) >> 9) & 0x01)
#define WDNS_FLAGS_RD(msg)		((((msg).flags) >> 8) & 0x01)
#define WDNS_FLAGS_RA(msg)		((((msg).flags) >> 7) & 0x01)
#define WDNS_FLAGS_Z(msg)		((((msg).flags) >> 6) & 0x01)
#define WDNS_FLAGS_AD(msg)		((((msg).flags) >> 5) & 0x01)
#define WDNS_FLAGS_CD(msg)		((((msg).flags) >> 4) & 0x01)
#define WDNS_FLAGS_RCODE(msg)		((msg).rcode)

/* Data structures and definitions. */

typedef enum {
	wdns_msg_success,
	wdns_msg_err_failure,
	wdns_msg_err_invalid_compression_pointer,
	wdns_msg_err_invalid_length_octet,
	wdns_msg_err_invalid_opcode,
	wdns_msg_err_invalid_rcode,
	wdns_msg_err_len,
	wdns_msg_err_malloc,
	wdns_msg_err_name_len,
	wdns_msg_err_name_overflow,
	wdns_msg_err_out_of_bounds,
	wdns_msg_err_overflow,
	wdns_msg_err_parse_error,
	wdns_msg_err_qdcount,
	wdns_msg_err_unknown_opcode,
	wdns_msg_err_unknown_rcode,
} wdns_msg_status;

typedef struct {
	uint8_t			len;
	uint8_t			*data;
} wdns_name_t;

typedef struct {
	uint16_t		len;
	uint8_t			data[];
} wdns_rdata_t;

typedef struct {
	uint32_t		rrttl;
	uint16_t		rrtype;
	uint16_t		rrclass;
	wdns_name_t		name;
	wdns_rdata_t		*rdata;
} wdns_rr_t;

typedef struct {
	uint32_t		rrttl;
	uint16_t		rrtype;
	uint16_t		rrclass;
	uint16_t		n_rdatas;
	wdns_name_t		name;
	wdns_rdata_t		**rdatas;
} wdns_rrset_t;

typedef struct {
	uint16_t		n_rrs;
	uint16_t		n_rrsets;
	wdns_rr_t		*rrs;
	wdns_rrset_t		*rrsets;
} wdns_rrset_array_t;

typedef struct {
	bool			present;
	uint8_t			version;
	uint16_t		flags;
	uint16_t		size;
	wdns_rdata_t		*options;
} wdns_edns_t;

typedef struct {
	wdns_rrset_array_t	sections[4];
	wdns_edns_t		edns;
	uint16_t		id;
	uint16_t		flags;
	uint16_t		rcode;
} wdns_message_t;

/* Function prototypes. */

typedef void (*wdns_callback_name)(wdns_name_t *name, void *user);

/* Functions for converting objects to presentation format strings. */

const char *	wdns_opcode_to_str(uint16_t dns_opcode);
const char *	wdns_rcode_to_str(uint16_t dns_rcode);
const char *	wdns_rrclass_to_str(uint16_t dns_class);
const char *	wdns_rrtype_to_str(uint16_t dns_type);
size_t		wdns_domain_to_str(const uint8_t *src, char *dst);

wdns_msg_status
wdns_rdata_to_str(const uint8_t *rdata, uint16_t rdata_len,
		  uint16_t rrtype, uint16_t rrclass,
		  char *dst, size_t *dstsz);

/* Functions for converting presentation format strings to objects. */

wdns_msg_status
wdns_str_to_name(const char *str, wdns_name_t *name);

/* Comparison functions. */

bool	wdns_compare_rr_rrset(const wdns_rr_t *rr, const wdns_rrset_t *rrset);

/* Functions for clearing wdns objects. */

void	wdns_clear_message(wdns_message_t *m);
void	wdns_clear_rr(wdns_rr_t *rr);
void	wdns_clear_rrset(wdns_rrset_t *rrset);
void	wdns_clear_rrset_array(wdns_rrset_array_t *a);

/* Functions for printing formatted output. */

void	wdns_print_message(FILE *fp, wdns_message_t *m);
void	wdns_print_rr(FILE *fp, wdns_rr_t *rr, unsigned sec);
void	wdns_print_rrset(FILE *fp, wdns_rrset_t *rrset, unsigned sec);
void	wdns_print_rrset_array(FILE *fp, wdns_rrset_array_t *a, unsigned sec);

/* Utility functions. */

size_t	wdns_skip_name(const uint8_t **data, const uint8_t *eod);

wdns_msg_status
wdns_copy_uname(const uint8_t *p, const uint8_t *eop, const uint8_t *src,
		uint8_t *dst, size_t *sz);

wdns_msg_status
wdns_insert_rr_rrset_array(wdns_rrset_array_t *a, wdns_rr_t *rr, unsigned sec);

wdns_msg_status
wdns_len_uname(const uint8_t *p, const uint8_t *eop, size_t *sz);

wdns_msg_status
wdns_sort_rrset(wdns_rrset_t *);

wdns_msg_status
wdns_unpack_name(const uint8_t *p, const uint8_t *eop, const uint8_t *src,
		 uint8_t *dst, size_t *sz);

wdns_msg_status
wdns_count_labels(wdns_name_t *name, size_t *nlabels);

wdns_msg_status
wdns_is_subdomain(wdns_name_t *n0, wdns_name_t *n1, bool *is_subdomain);

wdns_msg_status
wdns_file_load_names(const char *fname, wdns_callback_name cb, void *user);

wdns_msg_status
wdns_left_chop(wdns_name_t *name, wdns_name_t *chop);

/* Parsing functions. */

wdns_msg_status
wdns_parse_message(wdns_message_t *m, const uint8_t *pkt, size_t len);

wdns_msg_status
wdns_parse_rdata(const uint8_t *p, const uint8_t *eop, const uint8_t *ordata,
		 uint16_t rrtype, uint16_t rrclass, uint16_t rdlen,
		 size_t *alloc_bytes, uint8_t *dst);

/* Deserialization functions. */

wdns_msg_status
wdns_deserialize_rrset(wdns_rrset_t *rrset, const uint8_t *buf, size_t sz);

/* Serialization functions. */

wdns_msg_status
wdns_serialize_rrset(const wdns_rrset_t *rrset, uint8_t *buf, size_t *sz);

/* Downcasing functions. */

void	wdns_downcase_name(wdns_name_t *name);

wdns_msg_status
wdns_downcase_rdata(wdns_rdata_t *rdata, uint16_t rrtype, uint16_t rrclass);

wdns_msg_status
wdns_downcase_rrset(wdns_rrset_t *rrset);

#endif /* WDNS_MSG_H */