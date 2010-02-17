// Generated by the protocol buffer compiler.  DO NOT EDIT!

#define INTERNAL_SUPPRESS_PROTOBUF_FIELD_DEPRECATION
#include "lux/dist/update-protocol.pb.h"
#include <google/protobuf/stubs/once.h>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/wire_format_lite_inl.h>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/reflection_ops.h>
#include <google/protobuf/wire_format.h>
// @@protoc_insertion_point(includes)

namespace Lux {
namespace Protocol {

namespace {

const ::google::protobuf::Descriptor* Field_descriptor_ = NULL;
const ::google::protobuf::internal::GeneratedMessageReflection*
  Field_reflection_ = NULL;
const ::google::protobuf::Descriptor* Document_descriptor_ = NULL;
const ::google::protobuf::internal::GeneratedMessageReflection*
  Document_reflection_ = NULL;
const ::google::protobuf::Descriptor* Update_descriptor_ = NULL;
const ::google::protobuf::internal::GeneratedMessageReflection*
  Update_reflection_ = NULL;
const ::google::protobuf::EnumDescriptor* Update_UpdateType_descriptor_ = NULL;
const ::google::protobuf::Descriptor* Updates_descriptor_ = NULL;
const ::google::protobuf::internal::GeneratedMessageReflection*
  Updates_reflection_ = NULL;

}  // namespace


void protobuf_AssignDesc_lux_2fdist_2fupdate_2dprotocol_2eproto() {
  protobuf_AddDesc_lux_2fdist_2fupdate_2dprotocol_2eproto();
  const ::google::protobuf::FileDescriptor* file =
    ::google::protobuf::DescriptorPool::generated_pool()->FindFileByName(
      "lux/dist/update-protocol.proto");
  GOOGLE_CHECK(file != NULL);
  Field_descriptor_ = file->message_type(0);
  static const int Field_offsets_[2] = {
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(Field, name_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(Field, value_),
  };
  Field_reflection_ =
    new ::google::protobuf::internal::GeneratedMessageReflection(
      Field_descriptor_,
      Field::default_instance_,
      Field_offsets_,
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(Field, _has_bits_[0]),
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(Field, _unknown_fields_),
      -1,
      ::google::protobuf::DescriptorPool::generated_pool(),
      ::google::protobuf::MessageFactory::generated_factory(),
      sizeof(Field));
  Document_descriptor_ = file->message_type(1);
  static const int Document_offsets_[2] = {
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(Document, id_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(Document, field_),
  };
  Document_reflection_ =
    new ::google::protobuf::internal::GeneratedMessageReflection(
      Document_descriptor_,
      Document::default_instance_,
      Document_offsets_,
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(Document, _has_bits_[0]),
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(Document, _unknown_fields_),
      -1,
      ::google::protobuf::DescriptorPool::generated_pool(),
      ::google::protobuf::MessageFactory::generated_factory(),
      sizeof(Document));
  Update_descriptor_ = file->message_type(2);
  static const int Update_offsets_[2] = {
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(Update, document_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(Update, type_),
  };
  Update_reflection_ =
    new ::google::protobuf::internal::GeneratedMessageReflection(
      Update_descriptor_,
      Update::default_instance_,
      Update_offsets_,
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(Update, _has_bits_[0]),
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(Update, _unknown_fields_),
      -1,
      ::google::protobuf::DescriptorPool::generated_pool(),
      ::google::protobuf::MessageFactory::generated_factory(),
      sizeof(Update));
  Update_UpdateType_descriptor_ = Update_descriptor_->enum_type(0);
  Updates_descriptor_ = file->message_type(3);
  static const int Updates_offsets_[1] = {
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(Updates, update_),
  };
  Updates_reflection_ =
    new ::google::protobuf::internal::GeneratedMessageReflection(
      Updates_descriptor_,
      Updates::default_instance_,
      Updates_offsets_,
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(Updates, _has_bits_[0]),
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(Updates, _unknown_fields_),
      -1,
      ::google::protobuf::DescriptorPool::generated_pool(),
      ::google::protobuf::MessageFactory::generated_factory(),
      sizeof(Updates));
}

namespace {

GOOGLE_PROTOBUF_DECLARE_ONCE(protobuf_AssignDescriptors_once_);
inline void protobuf_AssignDescriptorsOnce() {
  ::google::protobuf::GoogleOnceInit(&protobuf_AssignDescriptors_once_,
                 &protobuf_AssignDesc_lux_2fdist_2fupdate_2dprotocol_2eproto);
}

void protobuf_RegisterTypes(const ::std::string&) {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedMessage(
    Field_descriptor_, &Field::default_instance());
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedMessage(
    Document_descriptor_, &Document::default_instance());
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedMessage(
    Update_descriptor_, &Update::default_instance());
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedMessage(
    Updates_descriptor_, &Updates::default_instance());
}

}  // namespace

void protobuf_ShutdownFile_lux_2fdist_2fupdate_2dprotocol_2eproto() {
  delete Field::default_instance_;
  delete Field_reflection_;
  delete Document::default_instance_;
  delete Document_reflection_;
  delete Update::default_instance_;
  delete Update_reflection_;
  delete Updates::default_instance_;
  delete Updates_reflection_;
}

void protobuf_AddDesc_lux_2fdist_2fupdate_2dprotocol_2eproto() {
  static bool already_here = false;
  if (already_here) return;
  already_here = true;
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  ::google::protobuf::DescriptorPool::InternalAddGeneratedFile(
    "\n\036lux/dist/update-protocol.proto\022\014Lux.Pr"
    "otocol\"$\n\005Field\022\014\n\004name\030\001 \002(\t\022\r\n\005value\030\002"
    " \002(\t\":\n\010Document\022\n\n\002id\030\001 \002(\t\022\"\n\005field\030\002 "
    "\003(\0132\023.Lux.Protocol.Field\"\217\001\n\006Update\022(\n\010d"
    "ocument\030\001 \002(\0132\026.Lux.Protocol.Document\022-\n"
    "\004type\030\002 \002(\0162\037.Lux.Protocol.Update.Update"
    "Type\",\n\nUpdateType\022\007\n\003ADD\020\000\022\n\n\006DELETE\020\001\022"
    "\t\n\005ALTER\020\002\"/\n\007Updates\022$\n\006update\030\001 \003(\0132\024."
    "Lux.Protocol.Update", 339);
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedFile(
    "lux/dist/update-protocol.proto", &protobuf_RegisterTypes);
  Field::default_instance_ = new Field();
  Document::default_instance_ = new Document();
  Update::default_instance_ = new Update();
  Updates::default_instance_ = new Updates();
  Field::default_instance_->InitAsDefaultInstance();
  Document::default_instance_->InitAsDefaultInstance();
  Update::default_instance_->InitAsDefaultInstance();
  Updates::default_instance_->InitAsDefaultInstance();
  ::google::protobuf::internal::OnShutdown(&protobuf_ShutdownFile_lux_2fdist_2fupdate_2dprotocol_2eproto);
}

// Force AddDescriptors() to be called at static initialization time.
struct StaticDescriptorInitializer_lux_2fdist_2fupdate_2dprotocol_2eproto {
  StaticDescriptorInitializer_lux_2fdist_2fupdate_2dprotocol_2eproto() {
    protobuf_AddDesc_lux_2fdist_2fupdate_2dprotocol_2eproto();
  }
} static_descriptor_initializer_lux_2fdist_2fupdate_2dprotocol_2eproto_;


// ===================================================================

const ::std::string Field::_default_name_;
const ::std::string Field::_default_value_;
#ifndef _MSC_VER
const int Field::kNameFieldNumber;
const int Field::kValueFieldNumber;
#endif  // !_MSC_VER

Field::Field()
  : ::google::protobuf::Message() {
  SharedCtor();
}

void Field::InitAsDefaultInstance() {
}

Field::Field(const Field& from)
  : ::google::protobuf::Message() {
  SharedCtor();
  MergeFrom(from);
}

void Field::SharedCtor() {
  _cached_size_ = 0;
  name_ = const_cast< ::std::string*>(&_default_name_);
  value_ = const_cast< ::std::string*>(&_default_value_);
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
}

Field::~Field() {
  SharedDtor();
}

void Field::SharedDtor() {
  if (name_ != &_default_name_) {
    delete name_;
  }
  if (value_ != &_default_value_) {
    delete value_;
  }
  if (this != default_instance_) {
  }
}

void Field::SetCachedSize(int size) const {
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
}
const ::google::protobuf::Descriptor* Field::descriptor() {
  protobuf_AssignDescriptorsOnce();
  return Field_descriptor_;
}

const Field& Field::default_instance() {
  if (default_instance_ == NULL) protobuf_AddDesc_lux_2fdist_2fupdate_2dprotocol_2eproto();  return *default_instance_;
}

Field* Field::default_instance_ = NULL;

Field* Field::New() const {
  return new Field;
}

void Field::Clear() {
  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    if (_has_bit(0)) {
      if (name_ != &_default_name_) {
        name_->clear();
      }
    }
    if (_has_bit(1)) {
      if (value_ != &_default_value_) {
        value_->clear();
      }
    }
  }
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
  mutable_unknown_fields()->Clear();
}

bool Field::MergePartialFromCodedStream(
    ::google::protobuf::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!(EXPRESSION)) return false
  ::google::protobuf::uint32 tag;
  while ((tag = input->ReadTag()) != 0) {
    switch (::google::protobuf::internal::WireFormatLite::GetTagFieldNumber(tag)) {
      // required string name = 1;
      case 1: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_LENGTH_DELIMITED) {
          DO_(::google::protobuf::internal::WireFormatLite::ReadString(
                input, this->mutable_name()));
          ::google::protobuf::internal::WireFormat::VerifyUTF8String(
            this->name().data(), this->name().length(),
            ::google::protobuf::internal::WireFormat::PARSE);
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(18)) goto parse_value;
        break;
      }
      
      // required string value = 2;
      case 2: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_LENGTH_DELIMITED) {
         parse_value:
          DO_(::google::protobuf::internal::WireFormatLite::ReadString(
                input, this->mutable_value()));
          ::google::protobuf::internal::WireFormat::VerifyUTF8String(
            this->value().data(), this->value().length(),
            ::google::protobuf::internal::WireFormat::PARSE);
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectAtEnd()) return true;
        break;
      }
      
      default: {
      handle_uninterpreted:
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_END_GROUP) {
          return true;
        }
        DO_(::google::protobuf::internal::WireFormat::SkipField(
              input, tag, mutable_unknown_fields()));
        break;
      }
    }
  }
  return true;
#undef DO_
}

void Field::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // required string name = 1;
  if (_has_bit(0)) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8String(
      this->name().data(), this->name().length(),
      ::google::protobuf::internal::WireFormat::SERIALIZE);
    ::google::protobuf::internal::WireFormatLite::WriteString(
      1, this->name(), output);
  }
  
  // required string value = 2;
  if (_has_bit(1)) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8String(
      this->value().data(), this->value().length(),
      ::google::protobuf::internal::WireFormat::SERIALIZE);
    ::google::protobuf::internal::WireFormatLite::WriteString(
      2, this->value(), output);
  }
  
  if (!unknown_fields().empty()) {
    ::google::protobuf::internal::WireFormat::SerializeUnknownFields(
        unknown_fields(), output);
  }
}

::google::protobuf::uint8* Field::SerializeWithCachedSizesToArray(
    ::google::protobuf::uint8* target) const {
  // required string name = 1;
  if (_has_bit(0)) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8String(
      this->name().data(), this->name().length(),
      ::google::protobuf::internal::WireFormat::SERIALIZE);
    target =
      ::google::protobuf::internal::WireFormatLite::WriteStringToArray(
        1, this->name(), target);
  }
  
  // required string value = 2;
  if (_has_bit(1)) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8String(
      this->value().data(), this->value().length(),
      ::google::protobuf::internal::WireFormat::SERIALIZE);
    target =
      ::google::protobuf::internal::WireFormatLite::WriteStringToArray(
        2, this->value(), target);
  }
  
  if (!unknown_fields().empty()) {
    target = ::google::protobuf::internal::WireFormat::SerializeUnknownFieldsToArray(
        unknown_fields(), target);
  }
  return target;
}

int Field::ByteSize() const {
  int total_size = 0;
  
  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    // required string name = 1;
    if (has_name()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::StringSize(
          this->name());
    }
    
    // required string value = 2;
    if (has_value()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::StringSize(
          this->value());
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

void Field::MergeFrom(const ::google::protobuf::Message& from) {
  GOOGLE_CHECK_NE(&from, this);
  const Field* source =
    ::google::protobuf::internal::dynamic_cast_if_available<const Field*>(
      &from);
  if (source == NULL) {
    ::google::protobuf::internal::ReflectionOps::Merge(from, this);
  } else {
    MergeFrom(*source);
  }
}

void Field::MergeFrom(const Field& from) {
  GOOGLE_CHECK_NE(&from, this);
  if (from._has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    if (from._has_bit(0)) {
      set_name(from.name());
    }
    if (from._has_bit(1)) {
      set_value(from.value());
    }
  }
  mutable_unknown_fields()->MergeFrom(from.unknown_fields());
}

void Field::CopyFrom(const ::google::protobuf::Message& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void Field::CopyFrom(const Field& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool Field::IsInitialized() const {
  if ((_has_bits_[0] & 0x00000003) != 0x00000003) return false;
  
  return true;
}

void Field::Swap(Field* other) {
  if (other != this) {
    std::swap(name_, other->name_);
    std::swap(value_, other->value_);
    std::swap(_has_bits_[0], other->_has_bits_[0]);
    _unknown_fields_.Swap(&other->_unknown_fields_);
    std::swap(_cached_size_, other->_cached_size_);
  }
}

::google::protobuf::Metadata Field::GetMetadata() const {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::Metadata metadata;
  metadata.descriptor = Field_descriptor_;
  metadata.reflection = Field_reflection_;
  return metadata;
}


// ===================================================================

const ::std::string Document::_default_id_;
#ifndef _MSC_VER
const int Document::kIdFieldNumber;
const int Document::kFieldFieldNumber;
#endif  // !_MSC_VER

Document::Document()
  : ::google::protobuf::Message() {
  SharedCtor();
}

void Document::InitAsDefaultInstance() {
}

Document::Document(const Document& from)
  : ::google::protobuf::Message() {
  SharedCtor();
  MergeFrom(from);
}

void Document::SharedCtor() {
  _cached_size_ = 0;
  id_ = const_cast< ::std::string*>(&_default_id_);
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
}

Document::~Document() {
  SharedDtor();
}

void Document::SharedDtor() {
  if (id_ != &_default_id_) {
    delete id_;
  }
  if (this != default_instance_) {
  }
}

void Document::SetCachedSize(int size) const {
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
}
const ::google::protobuf::Descriptor* Document::descriptor() {
  protobuf_AssignDescriptorsOnce();
  return Document_descriptor_;
}

const Document& Document::default_instance() {
  if (default_instance_ == NULL) protobuf_AddDesc_lux_2fdist_2fupdate_2dprotocol_2eproto();  return *default_instance_;
}

Document* Document::default_instance_ = NULL;

Document* Document::New() const {
  return new Document;
}

void Document::Clear() {
  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    if (_has_bit(0)) {
      if (id_ != &_default_id_) {
        id_->clear();
      }
    }
  }
  field_.Clear();
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
  mutable_unknown_fields()->Clear();
}

bool Document::MergePartialFromCodedStream(
    ::google::protobuf::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!(EXPRESSION)) return false
  ::google::protobuf::uint32 tag;
  while ((tag = input->ReadTag()) != 0) {
    switch (::google::protobuf::internal::WireFormatLite::GetTagFieldNumber(tag)) {
      // required string id = 1;
      case 1: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_LENGTH_DELIMITED) {
          DO_(::google::protobuf::internal::WireFormatLite::ReadString(
                input, this->mutable_id()));
          ::google::protobuf::internal::WireFormat::VerifyUTF8String(
            this->id().data(), this->id().length(),
            ::google::protobuf::internal::WireFormat::PARSE);
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(18)) goto parse_field;
        break;
      }
      
      // repeated .Lux.Protocol.Field field = 2;
      case 2: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_LENGTH_DELIMITED) {
         parse_field:
          DO_(::google::protobuf::internal::WireFormatLite::ReadMessageNoVirtual(
                input, add_field()));
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(18)) goto parse_field;
        if (input->ExpectAtEnd()) return true;
        break;
      }
      
      default: {
      handle_uninterpreted:
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_END_GROUP) {
          return true;
        }
        DO_(::google::protobuf::internal::WireFormat::SkipField(
              input, tag, mutable_unknown_fields()));
        break;
      }
    }
  }
  return true;
#undef DO_
}

void Document::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // required string id = 1;
  if (_has_bit(0)) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8String(
      this->id().data(), this->id().length(),
      ::google::protobuf::internal::WireFormat::SERIALIZE);
    ::google::protobuf::internal::WireFormatLite::WriteString(
      1, this->id(), output);
  }
  
  // repeated .Lux.Protocol.Field field = 2;
  for (int i = 0; i < this->field_size(); i++) {
    ::google::protobuf::internal::WireFormatLite::WriteMessageMaybeToArray(
      2, this->field(i), output);
  }
  
  if (!unknown_fields().empty()) {
    ::google::protobuf::internal::WireFormat::SerializeUnknownFields(
        unknown_fields(), output);
  }
}

::google::protobuf::uint8* Document::SerializeWithCachedSizesToArray(
    ::google::protobuf::uint8* target) const {
  // required string id = 1;
  if (_has_bit(0)) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8String(
      this->id().data(), this->id().length(),
      ::google::protobuf::internal::WireFormat::SERIALIZE);
    target =
      ::google::protobuf::internal::WireFormatLite::WriteStringToArray(
        1, this->id(), target);
  }
  
  // repeated .Lux.Protocol.Field field = 2;
  for (int i = 0; i < this->field_size(); i++) {
    target = ::google::protobuf::internal::WireFormatLite::
      WriteMessageNoVirtualToArray(
        2, this->field(i), target);
  }
  
  if (!unknown_fields().empty()) {
    target = ::google::protobuf::internal::WireFormat::SerializeUnknownFieldsToArray(
        unknown_fields(), target);
  }
  return target;
}

int Document::ByteSize() const {
  int total_size = 0;
  
  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    // required string id = 1;
    if (has_id()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::StringSize(
          this->id());
    }
    
  }
  // repeated .Lux.Protocol.Field field = 2;
  total_size += 1 * this->field_size();
  for (int i = 0; i < this->field_size(); i++) {
    total_size +=
      ::google::protobuf::internal::WireFormatLite::MessageSizeNoVirtual(
        this->field(i));
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

void Document::MergeFrom(const ::google::protobuf::Message& from) {
  GOOGLE_CHECK_NE(&from, this);
  const Document* source =
    ::google::protobuf::internal::dynamic_cast_if_available<const Document*>(
      &from);
  if (source == NULL) {
    ::google::protobuf::internal::ReflectionOps::Merge(from, this);
  } else {
    MergeFrom(*source);
  }
}

void Document::MergeFrom(const Document& from) {
  GOOGLE_CHECK_NE(&from, this);
  field_.MergeFrom(from.field_);
  if (from._has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    if (from._has_bit(0)) {
      set_id(from.id());
    }
  }
  mutable_unknown_fields()->MergeFrom(from.unknown_fields());
}

void Document::CopyFrom(const ::google::protobuf::Message& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void Document::CopyFrom(const Document& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool Document::IsInitialized() const {
  if ((_has_bits_[0] & 0x00000001) != 0x00000001) return false;
  
  for (int i = 0; i < field_size(); i++) {
    if (!this->field(i).IsInitialized()) return false;
  }
  return true;
}

void Document::Swap(Document* other) {
  if (other != this) {
    std::swap(id_, other->id_);
    field_.Swap(&other->field_);
    std::swap(_has_bits_[0], other->_has_bits_[0]);
    _unknown_fields_.Swap(&other->_unknown_fields_);
    std::swap(_cached_size_, other->_cached_size_);
  }
}

::google::protobuf::Metadata Document::GetMetadata() const {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::Metadata metadata;
  metadata.descriptor = Document_descriptor_;
  metadata.reflection = Document_reflection_;
  return metadata;
}


// ===================================================================

const ::google::protobuf::EnumDescriptor* Update_UpdateType_descriptor() {
  protobuf_AssignDescriptorsOnce();
  return Update_UpdateType_descriptor_;
}
bool Update_UpdateType_IsValid(int value) {
  switch(value) {
    case 0:
    case 1:
    case 2:
      return true;
    default:
      return false;
  }
}

#ifndef _MSC_VER
const Update_UpdateType Update::ADD;
const Update_UpdateType Update::DELETE;
const Update_UpdateType Update::ALTER;
const Update_UpdateType Update::UpdateType_MIN;
const Update_UpdateType Update::UpdateType_MAX;
const int Update::UpdateType_ARRAYSIZE;
#endif  // _MSC_VER
#ifndef _MSC_VER
const int Update::kDocumentFieldNumber;
const int Update::kTypeFieldNumber;
#endif  // !_MSC_VER

Update::Update()
  : ::google::protobuf::Message() {
  SharedCtor();
}

void Update::InitAsDefaultInstance() {
  document_ = const_cast< ::Lux::Protocol::Document*>(&::Lux::Protocol::Document::default_instance());
}

Update::Update(const Update& from)
  : ::google::protobuf::Message() {
  SharedCtor();
  MergeFrom(from);
}

void Update::SharedCtor() {
  _cached_size_ = 0;
  document_ = NULL;
  type_ = 0;
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
}

Update::~Update() {
  SharedDtor();
}

void Update::SharedDtor() {
  if (this != default_instance_) {
    delete document_;
  }
}

void Update::SetCachedSize(int size) const {
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
}
const ::google::protobuf::Descriptor* Update::descriptor() {
  protobuf_AssignDescriptorsOnce();
  return Update_descriptor_;
}

const Update& Update::default_instance() {
  if (default_instance_ == NULL) protobuf_AddDesc_lux_2fdist_2fupdate_2dprotocol_2eproto();  return *default_instance_;
}

Update* Update::default_instance_ = NULL;

Update* Update::New() const {
  return new Update;
}

void Update::Clear() {
  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    if (_has_bit(0)) {
      if (document_ != NULL) document_->::Lux::Protocol::Document::Clear();
    }
    type_ = 0;
  }
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
  mutable_unknown_fields()->Clear();
}

bool Update::MergePartialFromCodedStream(
    ::google::protobuf::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!(EXPRESSION)) return false
  ::google::protobuf::uint32 tag;
  while ((tag = input->ReadTag()) != 0) {
    switch (::google::protobuf::internal::WireFormatLite::GetTagFieldNumber(tag)) {
      // required .Lux.Protocol.Document document = 1;
      case 1: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_LENGTH_DELIMITED) {
          DO_(::google::protobuf::internal::WireFormatLite::ReadMessageNoVirtual(
               input, mutable_document()));
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(16)) goto parse_type;
        break;
      }
      
      // required .Lux.Protocol.Update.UpdateType type = 2;
      case 2: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
         parse_type:
          int value;
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   int, ::google::protobuf::internal::WireFormatLite::TYPE_ENUM>(
                 input, &value)));
          if (::Lux::Protocol::Update_UpdateType_IsValid(value)) {
            set_type(static_cast< ::Lux::Protocol::Update_UpdateType >(value));
          } else {
            mutable_unknown_fields()->AddVarint(2, value);
          }
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectAtEnd()) return true;
        break;
      }
      
      default: {
      handle_uninterpreted:
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_END_GROUP) {
          return true;
        }
        DO_(::google::protobuf::internal::WireFormat::SkipField(
              input, tag, mutable_unknown_fields()));
        break;
      }
    }
  }
  return true;
#undef DO_
}

void Update::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // required .Lux.Protocol.Document document = 1;
  if (_has_bit(0)) {
    ::google::protobuf::internal::WireFormatLite::WriteMessageMaybeToArray(
      1, this->document(), output);
  }
  
  // required .Lux.Protocol.Update.UpdateType type = 2;
  if (_has_bit(1)) {
    ::google::protobuf::internal::WireFormatLite::WriteEnum(
      2, this->type(), output);
  }
  
  if (!unknown_fields().empty()) {
    ::google::protobuf::internal::WireFormat::SerializeUnknownFields(
        unknown_fields(), output);
  }
}

::google::protobuf::uint8* Update::SerializeWithCachedSizesToArray(
    ::google::protobuf::uint8* target) const {
  // required .Lux.Protocol.Document document = 1;
  if (_has_bit(0)) {
    target = ::google::protobuf::internal::WireFormatLite::
      WriteMessageNoVirtualToArray(
        1, this->document(), target);
  }
  
  // required .Lux.Protocol.Update.UpdateType type = 2;
  if (_has_bit(1)) {
    target = ::google::protobuf::internal::WireFormatLite::WriteEnumToArray(
      2, this->type(), target);
  }
  
  if (!unknown_fields().empty()) {
    target = ::google::protobuf::internal::WireFormat::SerializeUnknownFieldsToArray(
        unknown_fields(), target);
  }
  return target;
}

int Update::ByteSize() const {
  int total_size = 0;
  
  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    // required .Lux.Protocol.Document document = 1;
    if (has_document()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::MessageSizeNoVirtual(
          this->document());
    }
    
    // required .Lux.Protocol.Update.UpdateType type = 2;
    if (has_type()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::EnumSize(this->type());
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

void Update::MergeFrom(const ::google::protobuf::Message& from) {
  GOOGLE_CHECK_NE(&from, this);
  const Update* source =
    ::google::protobuf::internal::dynamic_cast_if_available<const Update*>(
      &from);
  if (source == NULL) {
    ::google::protobuf::internal::ReflectionOps::Merge(from, this);
  } else {
    MergeFrom(*source);
  }
}

void Update::MergeFrom(const Update& from) {
  GOOGLE_CHECK_NE(&from, this);
  if (from._has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    if (from._has_bit(0)) {
      mutable_document()->::Lux::Protocol::Document::MergeFrom(from.document());
    }
    if (from._has_bit(1)) {
      set_type(from.type());
    }
  }
  mutable_unknown_fields()->MergeFrom(from.unknown_fields());
}

void Update::CopyFrom(const ::google::protobuf::Message& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void Update::CopyFrom(const Update& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool Update::IsInitialized() const {
  if ((_has_bits_[0] & 0x00000003) != 0x00000003) return false;
  
  if (has_document()) {
    if (!this->document().IsInitialized()) return false;
  }
  return true;
}

void Update::Swap(Update* other) {
  if (other != this) {
    std::swap(document_, other->document_);
    std::swap(type_, other->type_);
    std::swap(_has_bits_[0], other->_has_bits_[0]);
    _unknown_fields_.Swap(&other->_unknown_fields_);
    std::swap(_cached_size_, other->_cached_size_);
  }
}

::google::protobuf::Metadata Update::GetMetadata() const {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::Metadata metadata;
  metadata.descriptor = Update_descriptor_;
  metadata.reflection = Update_reflection_;
  return metadata;
}


// ===================================================================

#ifndef _MSC_VER
const int Updates::kUpdateFieldNumber;
#endif  // !_MSC_VER

Updates::Updates()
  : ::google::protobuf::Message() {
  SharedCtor();
}

void Updates::InitAsDefaultInstance() {
}

Updates::Updates(const Updates& from)
  : ::google::protobuf::Message() {
  SharedCtor();
  MergeFrom(from);
}

void Updates::SharedCtor() {
  _cached_size_ = 0;
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
}

Updates::~Updates() {
  SharedDtor();
}

void Updates::SharedDtor() {
  if (this != default_instance_) {
  }
}

void Updates::SetCachedSize(int size) const {
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
}
const ::google::protobuf::Descriptor* Updates::descriptor() {
  protobuf_AssignDescriptorsOnce();
  return Updates_descriptor_;
}

const Updates& Updates::default_instance() {
  if (default_instance_ == NULL) protobuf_AddDesc_lux_2fdist_2fupdate_2dprotocol_2eproto();  return *default_instance_;
}

Updates* Updates::default_instance_ = NULL;

Updates* Updates::New() const {
  return new Updates;
}

void Updates::Clear() {
  update_.Clear();
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
  mutable_unknown_fields()->Clear();
}

bool Updates::MergePartialFromCodedStream(
    ::google::protobuf::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!(EXPRESSION)) return false
  ::google::protobuf::uint32 tag;
  while ((tag = input->ReadTag()) != 0) {
    switch (::google::protobuf::internal::WireFormatLite::GetTagFieldNumber(tag)) {
      // repeated .Lux.Protocol.Update update = 1;
      case 1: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_LENGTH_DELIMITED) {
         parse_update:
          DO_(::google::protobuf::internal::WireFormatLite::ReadMessageNoVirtual(
                input, add_update()));
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(10)) goto parse_update;
        if (input->ExpectAtEnd()) return true;
        break;
      }
      
      default: {
      handle_uninterpreted:
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_END_GROUP) {
          return true;
        }
        DO_(::google::protobuf::internal::WireFormat::SkipField(
              input, tag, mutable_unknown_fields()));
        break;
      }
    }
  }
  return true;
#undef DO_
}

void Updates::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // repeated .Lux.Protocol.Update update = 1;
  for (int i = 0; i < this->update_size(); i++) {
    ::google::protobuf::internal::WireFormatLite::WriteMessageMaybeToArray(
      1, this->update(i), output);
  }
  
  if (!unknown_fields().empty()) {
    ::google::protobuf::internal::WireFormat::SerializeUnknownFields(
        unknown_fields(), output);
  }
}

::google::protobuf::uint8* Updates::SerializeWithCachedSizesToArray(
    ::google::protobuf::uint8* target) const {
  // repeated .Lux.Protocol.Update update = 1;
  for (int i = 0; i < this->update_size(); i++) {
    target = ::google::protobuf::internal::WireFormatLite::
      WriteMessageNoVirtualToArray(
        1, this->update(i), target);
  }
  
  if (!unknown_fields().empty()) {
    target = ::google::protobuf::internal::WireFormat::SerializeUnknownFieldsToArray(
        unknown_fields(), target);
  }
  return target;
}

int Updates::ByteSize() const {
  int total_size = 0;
  
  // repeated .Lux.Protocol.Update update = 1;
  total_size += 1 * this->update_size();
  for (int i = 0; i < this->update_size(); i++) {
    total_size +=
      ::google::protobuf::internal::WireFormatLite::MessageSizeNoVirtual(
        this->update(i));
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

void Updates::MergeFrom(const ::google::protobuf::Message& from) {
  GOOGLE_CHECK_NE(&from, this);
  const Updates* source =
    ::google::protobuf::internal::dynamic_cast_if_available<const Updates*>(
      &from);
  if (source == NULL) {
    ::google::protobuf::internal::ReflectionOps::Merge(from, this);
  } else {
    MergeFrom(*source);
  }
}

void Updates::MergeFrom(const Updates& from) {
  GOOGLE_CHECK_NE(&from, this);
  update_.MergeFrom(from.update_);
  mutable_unknown_fields()->MergeFrom(from.unknown_fields());
}

void Updates::CopyFrom(const ::google::protobuf::Message& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void Updates::CopyFrom(const Updates& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool Updates::IsInitialized() const {
  
  for (int i = 0; i < update_size(); i++) {
    if (!this->update(i).IsInitialized()) return false;
  }
  return true;
}

void Updates::Swap(Updates* other) {
  if (other != this) {
    update_.Swap(&other->update_);
    std::swap(_has_bits_[0], other->_has_bits_[0]);
    _unknown_fields_.Swap(&other->_unknown_fields_);
    std::swap(_cached_size_, other->_cached_size_);
  }
}

::google::protobuf::Metadata Updates::GetMetadata() const {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::Metadata metadata;
  metadata.descriptor = Updates_descriptor_;
  metadata.reflection = Updates_reflection_;
  return metadata;
}


// @@protoc_insertion_point(namespace_scope)

}  // namespace Protocol
}  // namespace Lux

// @@protoc_insertion_point(global_scope)
