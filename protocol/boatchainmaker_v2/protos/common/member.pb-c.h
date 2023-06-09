/* Generated by the protocol buffer compiler.  DO NOT EDIT! */
/* Generated from: member.proto */

#ifndef PROTOBUF_C_member_2eproto__INCLUDED
#define PROTOBUF_C_member_2eproto__INCLUDED

#include <protobuf-c/protobuf-c.h>

PROTOBUF_C__BEGIN_DECLS

#if PROTOBUF_C_VERSION_NUMBER < 1003000
# error This file was generated by a newer version of protoc-c which is incompatible with your libprotobuf-c headers. Please update your headers.
#elif 1003003 < PROTOBUF_C_MIN_COMPILER_VERSION
# error This file was generated by an older version of protoc-c which is incompatible with your libprotobuf-c headers. Please regenerate this file with a newer version of protoc-c.
#endif


typedef struct _Accesscontrol__Member Accesscontrol__Member;
typedef struct _Accesscontrol__MemberFull Accesscontrol__MemberFull;
typedef struct _Accesscontrol__MemberExtraData Accesscontrol__MemberExtraData;
typedef struct _Accesscontrol__MemberAndExtraData Accesscontrol__MemberAndExtraData;
typedef struct _Accesscontrol__PKInfo Accesscontrol__PKInfo;


/* --- enums --- */

typedef enum _Accesscontrol__MemberType {
  /*
   *X509 cert
   */
  ACCESSCONTROL__MEMBER_TYPE__CERT = 0,
  /*
   *cert hash
   */
  ACCESSCONTROL__MEMBER_TYPE__CERT_HASH = 1,
  /*
   *public key
   */
  ACCESSCONTROL__MEMBER_TYPE__PUBLIC_KEY = 2,
  /*
   *did
   */
  ACCESSCONTROL__MEMBER_TYPE__DID = 3,
  /*
   *alias
   */
  ACCESSCONTROL__MEMBER_TYPE__ALIAS = 4,
  /*
   *address
   */
  ACCESSCONTROL__MEMBER_TYPE__ADDR = 5
    PROTOBUF_C__FORCE_ENUM_TO_BE_INT_SIZE(ACCESSCONTROL__MEMBER_TYPE)
} Accesscontrol__MemberType;
/*
 * member status
 */
typedef enum _Accesscontrol__MemberStatus {
  /*
   *member's status is normal
   */
  ACCESSCONTROL__MEMBER_STATUS__NORMAL = 0,
  /*
   *member's status is invalid
   */
  ACCESSCONTROL__MEMBER_STATUS__INVALID = 1,
  /*
   *member's status is revoked
   */
  ACCESSCONTROL__MEMBER_STATUS__REVOKED = 2,
  /*
   *member's status is frozen
   */
  ACCESSCONTROL__MEMBER_STATUS__FROZEN = 3
    PROTOBUF_C__FORCE_ENUM_TO_BE_INT_SIZE(ACCESSCONTROL__MEMBER_STATUS)
} Accesscontrol__MemberStatus;
/*
 * verify the member's relevant identity material type
 */
typedef enum _Accesscontrol__VerifyType {
  /*
   *CRL
   */
  ACCESSCONTROL__VERIFY_TYPE__CRL = 0
    PROTOBUF_C__FORCE_ENUM_TO_BE_INT_SIZE(ACCESSCONTROL__VERIFY_TYPE)
} Accesscontrol__VerifyType;

/* --- messages --- */

/*
 * online member of blockchain
 */
struct  _Accesscontrol__Member
{
  ProtobufCMessage base;
  /*
   * organization identifier of the member
   */
  char *org_id;
  /*
   * member type
   */
  Accesscontrol__MemberType member_type;
  /*
   * member identity related info bytes
   */
  ProtobufCBinaryData member_info;
};
#define ACCESSCONTROL__MEMBER__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&accesscontrol__member__descriptor) \
    , (char *)protobuf_c_empty_string, ACCESSCONTROL__MEMBER_TYPE__CERT, {0,NULL} }


/*
 * full attribute member of blockchain
 */
struct  _Accesscontrol__MemberFull
{
  ProtobufCMessage base;
  /*
   * organization identifier of the member
   */
  char *org_id;
  /*
   * member type
   */
  Accesscontrol__MemberType member_type;
  /*
   * member identity related info bytes
   */
  ProtobufCBinaryData member_info;
  /*
   * the identity of this member (non-uniqueness)
   */
  char *member_id;
  /*
   * role of this member
   */
  char *role;
  /*
   * the identity of this member (unique)
   */
  char *uid;
};
#define ACCESSCONTROL__MEMBER_FULL__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&accesscontrol__member_full__descriptor) \
    , (char *)protobuf_c_empty_string, ACCESSCONTROL__MEMBER_TYPE__CERT, {0,NULL}, (char *)protobuf_c_empty_string, (char *)protobuf_c_empty_string, (char *)protobuf_c_empty_string }


/*
 * member extra data
 */
struct  _Accesscontrol__MemberExtraData
{
  ProtobufCMessage base;
  /*
   * sequence, like ethereum account nonce, by default is 0
   */
  /*
   *others
   */
  uint64_t sequence;
};
#define ACCESSCONTROL__MEMBER_EXTRA_DATA__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&accesscontrol__member_extra_data__descriptor) \
    , 0 }


struct  _Accesscontrol__MemberAndExtraData
{
  ProtobufCMessage base;
  Accesscontrol__Member *member;
  Accesscontrol__MemberExtraData *extra_data;
};
#define ACCESSCONTROL__MEMBER_AND_EXTRA_DATA__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&accesscontrol__member_and_extra_data__descriptor) \
    , NULL, NULL }


/*
 * public key member's info
 */
struct  _Accesscontrol__PKInfo
{
  ProtobufCMessage base;
  /*
   * the der of the public key
   */
  ProtobufCBinaryData pk_bytes;
  /*
   * member role
   */
  char *role;
  /*
   * member's org_id
   */
  char *org_id;
};
#define ACCESSCONTROL__PKINFO__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&accesscontrol__pkinfo__descriptor) \
    , {0,NULL}, (char *)protobuf_c_empty_string, (char *)protobuf_c_empty_string }


/* Accesscontrol__Member methods */
void   accesscontrol__member__init
                     (Accesscontrol__Member         *message);
size_t accesscontrol__member__get_packed_size
                     (const Accesscontrol__Member   *message);
size_t accesscontrol__member__pack
                     (const Accesscontrol__Member   *message,
                      uint8_t             *out);
size_t accesscontrol__member__pack_to_buffer
                     (const Accesscontrol__Member   *message,
                      ProtobufCBuffer     *buffer);
Accesscontrol__Member *
       accesscontrol__member__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   accesscontrol__member__free_unpacked
                     (Accesscontrol__Member *message,
                      ProtobufCAllocator *allocator);
/* Accesscontrol__MemberFull methods */
void   accesscontrol__member_full__init
                     (Accesscontrol__MemberFull         *message);
size_t accesscontrol__member_full__get_packed_size
                     (const Accesscontrol__MemberFull   *message);
size_t accesscontrol__member_full__pack
                     (const Accesscontrol__MemberFull   *message,
                      uint8_t             *out);
size_t accesscontrol__member_full__pack_to_buffer
                     (const Accesscontrol__MemberFull   *message,
                      ProtobufCBuffer     *buffer);
Accesscontrol__MemberFull *
       accesscontrol__member_full__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   accesscontrol__member_full__free_unpacked
                     (Accesscontrol__MemberFull *message,
                      ProtobufCAllocator *allocator);
/* Accesscontrol__MemberExtraData methods */
void   accesscontrol__member_extra_data__init
                     (Accesscontrol__MemberExtraData         *message);
size_t accesscontrol__member_extra_data__get_packed_size
                     (const Accesscontrol__MemberExtraData   *message);
size_t accesscontrol__member_extra_data__pack
                     (const Accesscontrol__MemberExtraData   *message,
                      uint8_t             *out);
size_t accesscontrol__member_extra_data__pack_to_buffer
                     (const Accesscontrol__MemberExtraData   *message,
                      ProtobufCBuffer     *buffer);
Accesscontrol__MemberExtraData *
       accesscontrol__member_extra_data__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   accesscontrol__member_extra_data__free_unpacked
                     (Accesscontrol__MemberExtraData *message,
                      ProtobufCAllocator *allocator);
/* Accesscontrol__MemberAndExtraData methods */
void   accesscontrol__member_and_extra_data__init
                     (Accesscontrol__MemberAndExtraData         *message);
size_t accesscontrol__member_and_extra_data__get_packed_size
                     (const Accesscontrol__MemberAndExtraData   *message);
size_t accesscontrol__member_and_extra_data__pack
                     (const Accesscontrol__MemberAndExtraData   *message,
                      uint8_t             *out);
size_t accesscontrol__member_and_extra_data__pack_to_buffer
                     (const Accesscontrol__MemberAndExtraData   *message,
                      ProtobufCBuffer     *buffer);
Accesscontrol__MemberAndExtraData *
       accesscontrol__member_and_extra_data__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   accesscontrol__member_and_extra_data__free_unpacked
                     (Accesscontrol__MemberAndExtraData *message,
                      ProtobufCAllocator *allocator);
/* Accesscontrol__PKInfo methods */
void   accesscontrol__pkinfo__init
                     (Accesscontrol__PKInfo         *message);
size_t accesscontrol__pkinfo__get_packed_size
                     (const Accesscontrol__PKInfo   *message);
size_t accesscontrol__pkinfo__pack
                     (const Accesscontrol__PKInfo   *message,
                      uint8_t             *out);
size_t accesscontrol__pkinfo__pack_to_buffer
                     (const Accesscontrol__PKInfo   *message,
                      ProtobufCBuffer     *buffer);
Accesscontrol__PKInfo *
       accesscontrol__pkinfo__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   accesscontrol__pkinfo__free_unpacked
                     (Accesscontrol__PKInfo *message,
                      ProtobufCAllocator *allocator);
/* --- per-message closures --- */

typedef void (*Accesscontrol__Member_Closure)
                 (const Accesscontrol__Member *message,
                  void *closure_data);
typedef void (*Accesscontrol__MemberFull_Closure)
                 (const Accesscontrol__MemberFull *message,
                  void *closure_data);
typedef void (*Accesscontrol__MemberExtraData_Closure)
                 (const Accesscontrol__MemberExtraData *message,
                  void *closure_data);
typedef void (*Accesscontrol__MemberAndExtraData_Closure)
                 (const Accesscontrol__MemberAndExtraData *message,
                  void *closure_data);
typedef void (*Accesscontrol__PKInfo_Closure)
                 (const Accesscontrol__PKInfo *message,
                  void *closure_data);

/* --- services --- */


/* --- descriptors --- */

extern const ProtobufCEnumDescriptor    accesscontrol__member_type__descriptor;
extern const ProtobufCEnumDescriptor    accesscontrol__member_status__descriptor;
extern const ProtobufCEnumDescriptor    accesscontrol__verify_type__descriptor;
extern const ProtobufCMessageDescriptor accesscontrol__member__descriptor;
extern const ProtobufCMessageDescriptor accesscontrol__member_full__descriptor;
extern const ProtobufCMessageDescriptor accesscontrol__member_extra_data__descriptor;
extern const ProtobufCMessageDescriptor accesscontrol__member_and_extra_data__descriptor;
extern const ProtobufCMessageDescriptor accesscontrol__pkinfo__descriptor;

PROTOBUF_C__END_DECLS


#endif  /* PROTOBUF_C_member_2eproto__INCLUDED */
