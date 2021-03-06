// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: test.proto

#define INTERNAL_SUPPRESS_PROTOBUF_FIELD_DEPRECATION
#include "test.pb.h"

#include <algorithm>

#include <google/protobuf/stubs/common.h>
#include <google/protobuf/stubs/once.h>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/wire_format_lite_inl.h>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/generated_message_reflection.h>
#include <google/protobuf/reflection_ops.h>
#include <google/protobuf/wire_format.h>
// @@protoc_insertion_point(includes)

namespace {

const ::google::protobuf::Descriptor* test_descriptor_ = NULL;
const ::google::protobuf::internal::GeneratedMessageReflection*
  test_reflection_ = NULL;
const ::google::protobuf::Descriptor* Certification_descriptor_ = NULL;
const ::google::protobuf::internal::GeneratedMessageReflection*
  Certification_reflection_ = NULL;
const ::google::protobuf::EnumDescriptor* Certification_TYPE_descriptor_ = NULL;
const ::google::protobuf::Descriptor* MessageBlank_descriptor_ = NULL;
const ::google::protobuf::internal::GeneratedMessageReflection*
  MessageBlank_reflection_ = NULL;

}  // namespace


void protobuf_AssignDesc_test_2eproto() {
  protobuf_AddDesc_test_2eproto();
  const ::google::protobuf::FileDescriptor* file =
    ::google::protobuf::DescriptorPool::generated_pool()->FindFileByName(
      "test.proto");
  GOOGLE_CHECK(file != NULL);
  test_descriptor_ = file->message_type(0);
  static const int test_offsets_[2] = {
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(test, result_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(test, str_),
  };
  test_reflection_ =
    new ::google::protobuf::internal::GeneratedMessageReflection(
      test_descriptor_,
      test::default_instance_,
      test_offsets_,
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(test, _has_bits_[0]),
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(test, _unknown_fields_),
      -1,
      ::google::protobuf::DescriptorPool::generated_pool(),
      ::google::protobuf::MessageFactory::generated_factory(),
      sizeof(test));
  Certification_descriptor_ = file->message_type(1);
  static const int Certification_offsets_[3] = {
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(Certification, type_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(Certification, name_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(Certification, code_),
  };
  Certification_reflection_ =
    new ::google::protobuf::internal::GeneratedMessageReflection(
      Certification_descriptor_,
      Certification::default_instance_,
      Certification_offsets_,
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(Certification, _has_bits_[0]),
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(Certification, _unknown_fields_),
      -1,
      ::google::protobuf::DescriptorPool::generated_pool(),
      ::google::protobuf::MessageFactory::generated_factory(),
      sizeof(Certification));
  Certification_TYPE_descriptor_ = Certification_descriptor_->enum_type(0);
  MessageBlank_descriptor_ = file->message_type(2);
  static const int MessageBlank_offsets_[1] = {
  };
  MessageBlank_reflection_ =
    new ::google::protobuf::internal::GeneratedMessageReflection(
      MessageBlank_descriptor_,
      MessageBlank::default_instance_,
      MessageBlank_offsets_,
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(MessageBlank, _has_bits_[0]),
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(MessageBlank, _unknown_fields_),
      -1,
      ::google::protobuf::DescriptorPool::generated_pool(),
      ::google::protobuf::MessageFactory::generated_factory(),
      sizeof(MessageBlank));
}

namespace {

GOOGLE_PROTOBUF_DECLARE_ONCE(protobuf_AssignDescriptors_once_);
inline void protobuf_AssignDescriptorsOnce() {
  ::google::protobuf::GoogleOnceInit(&protobuf_AssignDescriptors_once_,
                 &protobuf_AssignDesc_test_2eproto);
}

void protobuf_RegisterTypes(const ::std::string&) {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedMessage(
    test_descriptor_, &test::default_instance());
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedMessage(
    Certification_descriptor_, &Certification::default_instance());
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedMessage(
    MessageBlank_descriptor_, &MessageBlank::default_instance());
}

}  // namespace

void protobuf_ShutdownFile_test_2eproto() {
  delete test::default_instance_;
  delete test_reflection_;
  delete Certification::default_instance_;
  delete Certification_reflection_;
  delete MessageBlank::default_instance_;
  delete MessageBlank_reflection_;
}

void protobuf_AddDesc_test_2eproto() {
  static bool already_here = false;
  if (already_here) return;
  already_here = true;
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  ::google::protobuf::DescriptorPool::InternalAddGeneratedFile(
    "\n\ntest.proto\"#\n\004test\022\016\n\006result\030\001 \002(\005\022\013\n\003"
    "str\030\002 \002(\t\"p\n\rCertification\022!\n\004type\030\001 \002(\016"
    "2\023.Certification.TYPE\022\014\n\004name\030\002 \002(\t\022\014\n\004c"
    "ode\030\003 \002(\t\" \n\004TYPE\022\013\n\007eServer\020\001\022\013\n\007eClien"
    "t\020\002\"\016\n\014MessageBlank", 179);
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedFile(
    "test.proto", &protobuf_RegisterTypes);
  test::default_instance_ = new test();
  Certification::default_instance_ = new Certification();
  MessageBlank::default_instance_ = new MessageBlank();
  test::default_instance_->InitAsDefaultInstance();
  Certification::default_instance_->InitAsDefaultInstance();
  MessageBlank::default_instance_->InitAsDefaultInstance();
  ::google::protobuf::internal::OnShutdown(&protobuf_ShutdownFile_test_2eproto);
}

// Force AddDescriptors() to be called at static initialization time.
struct StaticDescriptorInitializer_test_2eproto {
  StaticDescriptorInitializer_test_2eproto() {
    protobuf_AddDesc_test_2eproto();
  }
} static_descriptor_initializer_test_2eproto_;

// ===================================================================

#ifndef _MSC_VER
const int test::kResultFieldNumber;
const int test::kStrFieldNumber;
#endif  // !_MSC_VER

test::test()
  : ::google::protobuf::Message() {
  SharedCtor();
  // @@protoc_insertion_point(constructor:test)
}

void test::InitAsDefaultInstance() {
}

test::test(const test& from)
  : ::google::protobuf::Message() {
  SharedCtor();
  MergeFrom(from);
  // @@protoc_insertion_point(copy_constructor:test)
}

void test::SharedCtor() {
  ::google::protobuf::internal::GetEmptyString();
  _cached_size_ = 0;
  result_ = 0;
  str_ = const_cast< ::std::string*>(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
}

test::~test() {
  // @@protoc_insertion_point(destructor:test)
  SharedDtor();
}

void test::SharedDtor() {
  if (str_ != &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    delete str_;
  }
  if (this != default_instance_) {
  }
}

void test::SetCachedSize(int size) const {
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
}
const ::google::protobuf::Descriptor* test::descriptor() {
  protobuf_AssignDescriptorsOnce();
  return test_descriptor_;
}

const test& test::default_instance() {
  if (default_instance_ == NULL) protobuf_AddDesc_test_2eproto();
  return *default_instance_;
}

test* test::default_instance_ = NULL;

test* test::New() const {
  return new test;
}

void test::Clear() {
  if (_has_bits_[0 / 32] & 3) {
    result_ = 0;
    if (has_str()) {
      if (str_ != &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
        str_->clear();
      }
    }
  }
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
  mutable_unknown_fields()->Clear();
}

bool test::MergePartialFromCodedStream(
    ::google::protobuf::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!(EXPRESSION)) goto failure
  ::google::protobuf::uint32 tag;
  // @@protoc_insertion_point(parse_start:test)
  for (;;) {
    ::std::pair< ::google::protobuf::uint32, bool> p = input->ReadTagWithCutoff(127);
    tag = p.first;
    if (!p.second) goto handle_unusual;
    switch (::google::protobuf::internal::WireFormatLite::GetTagFieldNumber(tag)) {
      // required int32 result = 1;
      case 1: {
        if (tag == 8) {
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::int32, ::google::protobuf::internal::WireFormatLite::TYPE_INT32>(
                 input, &result_)));
          set_has_result();
        } else {
          goto handle_unusual;
        }
        if (input->ExpectTag(18)) goto parse_str;
        break;
      }

      // required string str = 2;
      case 2: {
        if (tag == 18) {
         parse_str:
          DO_(::google::protobuf::internal::WireFormatLite::ReadString(
                input, this->mutable_str()));
          ::google::protobuf::internal::WireFormat::VerifyUTF8StringNamedField(
            this->str().data(), this->str().length(),
            ::google::protobuf::internal::WireFormat::PARSE,
            "str");
        } else {
          goto handle_unusual;
        }
        if (input->ExpectAtEnd()) goto success;
        break;
      }

      default: {
      handle_unusual:
        if (tag == 0 ||
            ::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_END_GROUP) {
          goto success;
        }
        DO_(::google::protobuf::internal::WireFormat::SkipField(
              input, tag, mutable_unknown_fields()));
        break;
      }
    }
  }
success:
  // @@protoc_insertion_point(parse_success:test)
  return true;
failure:
  // @@protoc_insertion_point(parse_failure:test)
  return false;
#undef DO_
}

void test::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // @@protoc_insertion_point(serialize_start:test)
  // required int32 result = 1;
  if (has_result()) {
    ::google::protobuf::internal::WireFormatLite::WriteInt32(1, this->result(), output);
  }

  // required string str = 2;
  if (has_str()) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8StringNamedField(
      this->str().data(), this->str().length(),
      ::google::protobuf::internal::WireFormat::SERIALIZE,
      "str");
    ::google::protobuf::internal::WireFormatLite::WriteStringMaybeAliased(
      2, this->str(), output);
  }

  if (!unknown_fields().empty()) {
    ::google::protobuf::internal::WireFormat::SerializeUnknownFields(
        unknown_fields(), output);
  }
  // @@protoc_insertion_point(serialize_end:test)
}

::google::protobuf::uint8* test::SerializeWithCachedSizesToArray(
    ::google::protobuf::uint8* target) const {
  // @@protoc_insertion_point(serialize_to_array_start:test)
  // required int32 result = 1;
  if (has_result()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteInt32ToArray(1, this->result(), target);
  }

  // required string str = 2;
  if (has_str()) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8StringNamedField(
      this->str().data(), this->str().length(),
      ::google::protobuf::internal::WireFormat::SERIALIZE,
      "str");
    target =
      ::google::protobuf::internal::WireFormatLite::WriteStringToArray(
        2, this->str(), target);
  }

  if (!unknown_fields().empty()) {
    target = ::google::protobuf::internal::WireFormat::SerializeUnknownFieldsToArray(
        unknown_fields(), target);
  }
  // @@protoc_insertion_point(serialize_to_array_end:test)
  return target;
}

int test::ByteSize() const {
  int total_size = 0;

  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    // required int32 result = 1;
    if (has_result()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::Int32Size(
          this->result());
    }

    // required string str = 2;
    if (has_str()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::StringSize(
          this->str());
    }

  }
  if (!unknown_fields().empty()) {
    total_size +=
      ::google::protobuf::internal::WireFormat::ComputeUnknownFieldsSize(
        unknown_fields());
  }
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = total_size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
  return total_size;
}

void test::MergeFrom(const ::google::protobuf::Message& from) {
  GOOGLE_CHECK_NE(&from, this);
  const test* source =
    ::google::protobuf::internal::dynamic_cast_if_available<const test*>(
      &from);
  if (source == NULL) {
    ::google::protobuf::internal::ReflectionOps::Merge(from, this);
  } else {
    MergeFrom(*source);
  }
}

void test::MergeFrom(const test& from) {
  GOOGLE_CHECK_NE(&from, this);
  if (from._has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    if (from.has_result()) {
      set_result(from.result());
    }
    if (from.has_str()) {
      set_str(from.str());
    }
  }
  mutable_unknown_fields()->MergeFrom(from.unknown_fields());
}

void test::CopyFrom(const ::google::protobuf::Message& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void test::CopyFrom(const test& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool test::IsInitialized() const {
  if ((_has_bits_[0] & 0x00000003) != 0x00000003) return false;

  return true;
}

void test::Swap(test* other) {
  if (other != this) {
    std::swap(result_, other->result_);
    std::swap(str_, other->str_);
    std::swap(_has_bits_[0], other->_has_bits_[0]);
    _unknown_fields_.Swap(&other->_unknown_fields_);
    std::swap(_cached_size_, other->_cached_size_);
  }
}

::google::protobuf::Metadata test::GetMetadata() const {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::Metadata metadata;
  metadata.descriptor = test_descriptor_;
  metadata.reflection = test_reflection_;
  return metadata;
}


// ===================================================================

const ::google::protobuf::EnumDescriptor* Certification_TYPE_descriptor() {
  protobuf_AssignDescriptorsOnce();
  return Certification_TYPE_descriptor_;
}
bool Certification_TYPE_IsValid(int value) {
  switch(value) {
    case 1:
    case 2:
      return true;
    default:
      return false;
  }
}

#ifndef _MSC_VER
const Certification_TYPE Certification::eServer;
const Certification_TYPE Certification::eClient;
const Certification_TYPE Certification::TYPE_MIN;
const Certification_TYPE Certification::TYPE_MAX;
const int Certification::TYPE_ARRAYSIZE;
#endif  // _MSC_VER
#ifndef _MSC_VER
const int Certification::kTypeFieldNumber;
const int Certification::kNameFieldNumber;
const int Certification::kCodeFieldNumber;
#endif  // !_MSC_VER

Certification::Certification()
  : ::google::protobuf::Message() {
  SharedCtor();
  // @@protoc_insertion_point(constructor:Certification)
}

void Certification::InitAsDefaultInstance() {
}

Certification::Certification(const Certification& from)
  : ::google::protobuf::Message() {
  SharedCtor();
  MergeFrom(from);
  // @@protoc_insertion_point(copy_constructor:Certification)
}

void Certification::SharedCtor() {
  ::google::protobuf::internal::GetEmptyString();
  _cached_size_ = 0;
  type_ = 1;
  name_ = const_cast< ::std::string*>(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
  code_ = const_cast< ::std::string*>(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
}

Certification::~Certification() {
  // @@protoc_insertion_point(destructor:Certification)
  SharedDtor();
}

void Certification::SharedDtor() {
  if (name_ != &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    delete name_;
  }
  if (code_ != &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    delete code_;
  }
  if (this != default_instance_) {
  }
}

void Certification::SetCachedSize(int size) const {
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
}
const ::google::protobuf::Descriptor* Certification::descriptor() {
  protobuf_AssignDescriptorsOnce();
  return Certification_descriptor_;
}

const Certification& Certification::default_instance() {
  if (default_instance_ == NULL) protobuf_AddDesc_test_2eproto();
  return *default_instance_;
}

Certification* Certification::default_instance_ = NULL;

Certification* Certification::New() const {
  return new Certification;
}

void Certification::Clear() {
  if (_has_bits_[0 / 32] & 7) {
    type_ = 1;
    if (has_name()) {
      if (name_ != &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
        name_->clear();
      }
    }
    if (has_code()) {
      if (code_ != &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
        code_->clear();
      }
    }
  }
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
  mutable_unknown_fields()->Clear();
}

bool Certification::MergePartialFromCodedStream(
    ::google::protobuf::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!(EXPRESSION)) goto failure
  ::google::protobuf::uint32 tag;
  // @@protoc_insertion_point(parse_start:Certification)
  for (;;) {
    ::std::pair< ::google::protobuf::uint32, bool> p = input->ReadTagWithCutoff(127);
    tag = p.first;
    if (!p.second) goto handle_unusual;
    switch (::google::protobuf::internal::WireFormatLite::GetTagFieldNumber(tag)) {
      // required .Certification.TYPE type = 1;
      case 1: {
        if (tag == 8) {
          int value;
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   int, ::google::protobuf::internal::WireFormatLite::TYPE_ENUM>(
                 input, &value)));
          if (::Certification_TYPE_IsValid(value)) {
            set_type(static_cast< ::Certification_TYPE >(value));
          } else {
            mutable_unknown_fields()->AddVarint(1, value);
          }
        } else {
          goto handle_unusual;
        }
        if (input->ExpectTag(18)) goto parse_name;
        break;
      }

      // required string name = 2;
      case 2: {
        if (tag == 18) {
         parse_name:
          DO_(::google::protobuf::internal::WireFormatLite::ReadString(
                input, this->mutable_name()));
          ::google::protobuf::internal::WireFormat::VerifyUTF8StringNamedField(
            this->name().data(), this->name().length(),
            ::google::protobuf::internal::WireFormat::PARSE,
            "name");
        } else {
          goto handle_unusual;
        }
        if (input->ExpectTag(26)) goto parse_code;
        break;
      }

      // required string code = 3;
      case 3: {
        if (tag == 26) {
         parse_code:
          DO_(::google::protobuf::internal::WireFormatLite::ReadString(
                input, this->mutable_code()));
          ::google::protobuf::internal::WireFormat::VerifyUTF8StringNamedField(
            this->code().data(), this->code().length(),
            ::google::protobuf::internal::WireFormat::PARSE,
            "code");
        } else {
          goto handle_unusual;
        }
        if (input->ExpectAtEnd()) goto success;
        break;
      }

      default: {
      handle_unusual:
        if (tag == 0 ||
            ::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_END_GROUP) {
          goto success;
        }
        DO_(::google::protobuf::internal::WireFormat::SkipField(
              input, tag, mutable_unknown_fields()));
        break;
      }
    }
  }
success:
  // @@protoc_insertion_point(parse_success:Certification)
  return true;
failure:
  // @@protoc_insertion_point(parse_failure:Certification)
  return false;
#undef DO_
}

void Certification::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // @@protoc_insertion_point(serialize_start:Certification)
  // required .Certification.TYPE type = 1;
  if (has_type()) {
    ::google::protobuf::internal::WireFormatLite::WriteEnum(
      1, this->type(), output);
  }

  // required string name = 2;
  if (has_name()) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8StringNamedField(
      this->name().data(), this->name().length(),
      ::google::protobuf::internal::WireFormat::SERIALIZE,
      "name");
    ::google::protobuf::internal::WireFormatLite::WriteStringMaybeAliased(
      2, this->name(), output);
  }

  // required string code = 3;
  if (has_code()) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8StringNamedField(
      this->code().data(), this->code().length(),
      ::google::protobuf::internal::WireFormat::SERIALIZE,
      "code");
    ::google::protobuf::internal::WireFormatLite::WriteStringMaybeAliased(
      3, this->code(), output);
  }

  if (!unknown_fields().empty()) {
    ::google::protobuf::internal::WireFormat::SerializeUnknownFields(
        unknown_fields(), output);
  }
  // @@protoc_insertion_point(serialize_end:Certification)
}

::google::protobuf::uint8* Certification::SerializeWithCachedSizesToArray(
    ::google::protobuf::uint8* target) const {
  // @@protoc_insertion_point(serialize_to_array_start:Certification)
  // required .Certification.TYPE type = 1;
  if (has_type()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteEnumToArray(
      1, this->type(), target);
  }

  // required string name = 2;
  if (has_name()) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8StringNamedField(
      this->name().data(), this->name().length(),
      ::google::protobuf::internal::WireFormat::SERIALIZE,
      "name");
    target =
      ::google::protobuf::internal::WireFormatLite::WriteStringToArray(
        2, this->name(), target);
  }

  // required string code = 3;
  if (has_code()) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8StringNamedField(
      this->code().data(), this->code().length(),
      ::google::protobuf::internal::WireFormat::SERIALIZE,
      "code");
    target =
      ::google::protobuf::internal::WireFormatLite::WriteStringToArray(
        3, this->code(), target);
  }

  if (!unknown_fields().empty()) {
    target = ::google::protobuf::internal::WireFormat::SerializeUnknownFieldsToArray(
        unknown_fields(), target);
  }
  // @@protoc_insertion_point(serialize_to_array_end:Certification)
  return target;
}

int Certification::ByteSize() const {
  int total_size = 0;

  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    // required .Certification.TYPE type = 1;
    if (has_type()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::EnumSize(this->type());
    }

    // required string name = 2;
    if (has_name()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::StringSize(
          this->name());
    }

    // required string code = 3;
    if (has_code()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::StringSize(
          this->code());
    }

  }
  if (!unknown_fields().empty()) {
    total_size +=
      ::google::protobuf::internal::WireFormat::ComputeUnknownFieldsSize(
        unknown_fields());
  }
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = total_size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
  return total_size;
}

void Certification::MergeFrom(const ::google::protobuf::Message& from) {
  GOOGLE_CHECK_NE(&from, this);
  const Certification* source =
    ::google::protobuf::internal::dynamic_cast_if_available<const Certification*>(
      &from);
  if (source == NULL) {
    ::google::protobuf::internal::ReflectionOps::Merge(from, this);
  } else {
    MergeFrom(*source);
  }
}

void Certification::MergeFrom(const Certification& from) {
  GOOGLE_CHECK_NE(&from, this);
  if (from._has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    if (from.has_type()) {
      set_type(from.type());
    }
    if (from.has_name()) {
      set_name(from.name());
    }
    if (from.has_code()) {
      set_code(from.code());
    }
  }
  mutable_unknown_fields()->MergeFrom(from.unknown_fields());
}

void Certification::CopyFrom(const ::google::protobuf::Message& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void Certification::CopyFrom(const Certification& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool Certification::IsInitialized() const {
  if ((_has_bits_[0] & 0x00000007) != 0x00000007) return false;

  return true;
}

void Certification::Swap(Certification* other) {
  if (other != this) {
    std::swap(type_, other->type_);
    std::swap(name_, other->name_);
    std::swap(code_, other->code_);
    std::swap(_has_bits_[0], other->_has_bits_[0]);
    _unknown_fields_.Swap(&other->_unknown_fields_);
    std::swap(_cached_size_, other->_cached_size_);
  }
}

::google::protobuf::Metadata Certification::GetMetadata() const {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::Metadata metadata;
  metadata.descriptor = Certification_descriptor_;
  metadata.reflection = Certification_reflection_;
  return metadata;
}


// ===================================================================

#ifndef _MSC_VER
#endif  // !_MSC_VER

MessageBlank::MessageBlank()
  : ::google::protobuf::Message() {
  SharedCtor();
  // @@protoc_insertion_point(constructor:MessageBlank)
}

void MessageBlank::InitAsDefaultInstance() {
}

MessageBlank::MessageBlank(const MessageBlank& from)
  : ::google::protobuf::Message() {
  SharedCtor();
  MergeFrom(from);
  // @@protoc_insertion_point(copy_constructor:MessageBlank)
}

void MessageBlank::SharedCtor() {
  _cached_size_ = 0;
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
}

MessageBlank::~MessageBlank() {
  // @@protoc_insertion_point(destructor:MessageBlank)
  SharedDtor();
}

void MessageBlank::SharedDtor() {
  if (this != default_instance_) {
  }
}

void MessageBlank::SetCachedSize(int size) const {
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
}
const ::google::protobuf::Descriptor* MessageBlank::descriptor() {
  protobuf_AssignDescriptorsOnce();
  return MessageBlank_descriptor_;
}

const MessageBlank& MessageBlank::default_instance() {
  if (default_instance_ == NULL) protobuf_AddDesc_test_2eproto();
  return *default_instance_;
}

MessageBlank* MessageBlank::default_instance_ = NULL;

MessageBlank* MessageBlank::New() const {
  return new MessageBlank;
}

void MessageBlank::Clear() {
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
  mutable_unknown_fields()->Clear();
}

bool MessageBlank::MergePartialFromCodedStream(
    ::google::protobuf::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!(EXPRESSION)) goto failure
  ::google::protobuf::uint32 tag;
  // @@protoc_insertion_point(parse_start:MessageBlank)
  for (;;) {
    ::std::pair< ::google::protobuf::uint32, bool> p = input->ReadTagWithCutoff(127);
    tag = p.first;
    if (!p.second) goto handle_unusual;
  handle_unusual:
    if (tag == 0 ||
        ::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
        ::google::protobuf::internal::WireFormatLite::WIRETYPE_END_GROUP) {
      goto success;
    }
    DO_(::google::protobuf::internal::WireFormat::SkipField(
          input, tag, mutable_unknown_fields()));
  }
success:
  // @@protoc_insertion_point(parse_success:MessageBlank)
  return true;
failure:
  // @@protoc_insertion_point(parse_failure:MessageBlank)
  return false;
#undef DO_
}

void MessageBlank::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // @@protoc_insertion_point(serialize_start:MessageBlank)
  if (!unknown_fields().empty()) {
    ::google::protobuf::internal::WireFormat::SerializeUnknownFields(
        unknown_fields(), output);
  }
  // @@protoc_insertion_point(serialize_end:MessageBlank)
}

::google::protobuf::uint8* MessageBlank::SerializeWithCachedSizesToArray(
    ::google::protobuf::uint8* target) const {
  // @@protoc_insertion_point(serialize_to_array_start:MessageBlank)
  if (!unknown_fields().empty()) {
    target = ::google::protobuf::internal::WireFormat::SerializeUnknownFieldsToArray(
        unknown_fields(), target);
  }
  // @@protoc_insertion_point(serialize_to_array_end:MessageBlank)
  return target;
}

int MessageBlank::ByteSize() const {
  int total_size = 0;

  if (!unknown_fields().empty()) {
    total_size +=
      ::google::protobuf::internal::WireFormat::ComputeUnknownFieldsSize(
        unknown_fields());
  }
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = total_size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
  return total_size;
}

void MessageBlank::MergeFrom(const ::google::protobuf::Message& from) {
  GOOGLE_CHECK_NE(&from, this);
  const MessageBlank* source =
    ::google::protobuf::internal::dynamic_cast_if_available<const MessageBlank*>(
      &from);
  if (source == NULL) {
    ::google::protobuf::internal::ReflectionOps::Merge(from, this);
  } else {
    MergeFrom(*source);
  }
}

void MessageBlank::MergeFrom(const MessageBlank& from) {
  GOOGLE_CHECK_NE(&from, this);
  mutable_unknown_fields()->MergeFrom(from.unknown_fields());
}

void MessageBlank::CopyFrom(const ::google::protobuf::Message& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void MessageBlank::CopyFrom(const MessageBlank& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool MessageBlank::IsInitialized() const {

  return true;
}

void MessageBlank::Swap(MessageBlank* other) {
  if (other != this) {
    _unknown_fields_.Swap(&other->_unknown_fields_);
    std::swap(_cached_size_, other->_cached_size_);
  }
}

::google::protobuf::Metadata MessageBlank::GetMetadata() const {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::Metadata metadata;
  metadata.descriptor = MessageBlank_descriptor_;
  metadata.reflection = MessageBlank_reflection_;
  return metadata;
}


// @@protoc_insertion_point(namespace_scope)

// @@protoc_insertion_point(global_scope)
