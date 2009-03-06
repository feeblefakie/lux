// Generated by the protocol buffer compiler.  DO NOT EDIT!

#include "update-protocol.pb.h"
#include <google/protobuf/descriptor.h>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/reflection_ops.h>
#include <google/protobuf/wire_format_inl.h>

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


void protobuf_BuildDesc_update_2dprotocol_2eproto_AssignGlobalDescriptors(const ::google::protobuf::FileDescriptor* file) {
  Field_descriptor_ = file->message_type(0);
  Field::default_instance_ = new Field();
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
      sizeof(Field));
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedMessage(
    Field_descriptor_, Field::default_instance_);
  Document_descriptor_ = file->message_type(1);
  Document::default_instance_ = new Document();
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
      sizeof(Document));
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedMessage(
    Document_descriptor_, Document::default_instance_);
  Update_descriptor_ = file->message_type(2);
  Update::default_instance_ = new Update();
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
      sizeof(Update));
  Update_UpdateType_descriptor_ = Update_descriptor_->enum_type(0);
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedMessage(
    Update_descriptor_, Update::default_instance_);
  Updates_descriptor_ = file->message_type(3);
  Updates::default_instance_ = new Updates();
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
      sizeof(Updates));
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedMessage(
    Updates_descriptor_, Updates::default_instance_);
  Field::default_instance_->InitAsDefaultInstance();
  Document::default_instance_->InitAsDefaultInstance();
  Update::default_instance_->InitAsDefaultInstance();
  Updates::default_instance_->InitAsDefaultInstance();
}

void protobuf_BuildDesc_update_2dprotocol_2eproto() {
  static bool already_here = false;
  if (already_here) return;
  already_here = true;
  GOOGLE_PROTOBUF_VERIFY_VERSION;
  ::google::protobuf::DescriptorPool* pool =
    ::google::protobuf::DescriptorPool::internal_generated_pool();

  pool->InternalBuildGeneratedFile(
    "\n\025update-protocol.proto\022\014Lux.Protocol\"$\n"
    "\005Field\022\014\n\004name\030\001 \002(\t\022\r\n\005value\030\002 \002(\t\":\n\010D"
    "ocument\022\n\n\002id\030\001 \002(\t\022\"\n\005field\030\002 \003(\0132\023.Lux"
    ".Protocol.Field\"\217\001\n\006Update\022(\n\010document\030\001"
    " \002(\0132\026.Lux.Protocol.Document\022-\n\004type\030\002 \002"
    "(\0162\037.Lux.Protocol.Update.UpdateType\",\n\nU"
    "pdateType\022\007\n\003ADD\020\000\022\n\n\006DELETE\020\001\022\t\n\005ALTER\020"
    "\002\"/\n\007Updates\022$\n\006update\030\001 \003(\0132\024.Lux.Proto"
    "col.Update", 330,
  &protobuf_BuildDesc_update_2dprotocol_2eproto_AssignGlobalDescriptors);
}

// Force BuildDescriptors() to be called at static initialization time.
struct StaticDescriptorInitializer_update_2dprotocol_2eproto {
  StaticDescriptorInitializer_update_2dprotocol_2eproto() {
    protobuf_BuildDesc_update_2dprotocol_2eproto();
  }
} static_descriptor_initializer_update_2dprotocol_2eproto_;


// ===================================================================

const ::std::string Field::_default_name_;
const ::std::string Field::_default_value_;
Field::Field()
  : ::google::protobuf::Message(),
    _cached_size_(0),
    name_(const_cast< ::std::string*>(&_default_name_)),
    value_(const_cast< ::std::string*>(&_default_value_)) {
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
}

void Field::InitAsDefaultInstance() {}

Field::Field(const Field& from)
  : ::google::protobuf::Message(),
    _cached_size_(0),
    name_(const_cast< ::std::string*>(&_default_name_)),
    value_(const_cast< ::std::string*>(&_default_value_)) {
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
  MergeFrom(from);
}

Field::~Field() {
  if (name_ != &_default_name_) {
    delete name_;
  }
  if (value_ != &_default_value_) {
    delete value_;
  }
  if (this != default_instance_) {
  }
}

const ::google::protobuf::Descriptor* Field::descriptor() {
  if (Field_descriptor_ == NULL) protobuf_BuildDesc_update_2dprotocol_2eproto();
  return Field_descriptor_;
}

const Field& Field::default_instance() {
  if (default_instance_ == NULL) protobuf_BuildDesc_update_2dprotocol_2eproto();
  return *default_instance_;
}

Field* Field::default_instance_ = NULL;

Field* Field::New() const {
  return new Field;
}

const ::google::protobuf::Descriptor* Field::GetDescriptor() const {
  return descriptor();
}

const ::google::protobuf::Reflection* Field::GetReflection() const {
  if (Field_reflection_ == NULL) protobuf_BuildDesc_update_2dprotocol_2eproto();
  return Field_reflection_;
}

// ===================================================================

const ::std::string Document::_default_id_;

Document::Document()
  : ::google::protobuf::Message(),
    _cached_size_(0),
    id_(const_cast< ::std::string*>(&_default_id_)) {
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
}

void Document::InitAsDefaultInstance() {}

Document::Document(const Document& from)
  : ::google::protobuf::Message(),
    _cached_size_(0),
    id_(const_cast< ::std::string*>(&_default_id_)) {
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
  MergeFrom(from);
}

Document::~Document() {
  if (id_ != &_default_id_) {
    delete id_;
  }
  if (this != default_instance_) {
  }
}

const ::google::protobuf::Descriptor* Document::descriptor() {
  if (Document_descriptor_ == NULL) protobuf_BuildDesc_update_2dprotocol_2eproto();
  return Document_descriptor_;
}

const Document& Document::default_instance() {
  if (default_instance_ == NULL) protobuf_BuildDesc_update_2dprotocol_2eproto();
  return *default_instance_;
}

Document* Document::default_instance_ = NULL;

Document* Document::New() const {
  return new Document;
}

const ::google::protobuf::Descriptor* Document::GetDescriptor() const {
  return descriptor();
}

const ::google::protobuf::Reflection* Document::GetReflection() const {
  if (Document_reflection_ == NULL) protobuf_BuildDesc_update_2dprotocol_2eproto();
  return Document_reflection_;
}

// ===================================================================

const ::google::protobuf::EnumDescriptor* Update_UpdateType_descriptor() {
  if (Update_UpdateType_descriptor_ == NULL) protobuf_BuildDesc_update_2dprotocol_2eproto();
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
#endif  // _MSC_VER


Update::Update()
  : ::google::protobuf::Message(),
    _cached_size_(0),
    document_(NULL),
    type_(0) {
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
}

void Update::InitAsDefaultInstance() {  document_ = const_cast< ::Lux::Protocol::Document*>(&::Lux::Protocol::Document::default_instance());
}

Update::Update(const Update& from)
  : ::google::protobuf::Message(),
    _cached_size_(0),
    document_(NULL),
    type_(0) {
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
  MergeFrom(from);
}

Update::~Update() {
  if (this != default_instance_) {
    delete document_;
  }
}

const ::google::protobuf::Descriptor* Update::descriptor() {
  if (Update_descriptor_ == NULL) protobuf_BuildDesc_update_2dprotocol_2eproto();
  return Update_descriptor_;
}

const Update& Update::default_instance() {
  if (default_instance_ == NULL) protobuf_BuildDesc_update_2dprotocol_2eproto();
  return *default_instance_;
}

Update* Update::default_instance_ = NULL;

Update* Update::New() const {
  return new Update;
}

const ::google::protobuf::Descriptor* Update::GetDescriptor() const {
  return descriptor();
}

const ::google::protobuf::Reflection* Update::GetReflection() const {
  if (Update_reflection_ == NULL) protobuf_BuildDesc_update_2dprotocol_2eproto();
  return Update_reflection_;
}

// ===================================================================


Updates::Updates()
  : ::google::protobuf::Message(),
    _cached_size_(0) {
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
}

void Updates::InitAsDefaultInstance() {}

Updates::Updates(const Updates& from)
  : ::google::protobuf::Message(),
    _cached_size_(0) {
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
  MergeFrom(from);
}

Updates::~Updates() {
  if (this != default_instance_) {
  }
}

const ::google::protobuf::Descriptor* Updates::descriptor() {
  if (Updates_descriptor_ == NULL) protobuf_BuildDesc_update_2dprotocol_2eproto();
  return Updates_descriptor_;
}

const Updates& Updates::default_instance() {
  if (default_instance_ == NULL) protobuf_BuildDesc_update_2dprotocol_2eproto();
  return *default_instance_;
}

Updates* Updates::default_instance_ = NULL;

Updates* Updates::New() const {
  return new Updates;
}

const ::google::protobuf::Descriptor* Updates::GetDescriptor() const {
  return descriptor();
}

const ::google::protobuf::Reflection* Updates::GetReflection() const {
  if (Updates_reflection_ == NULL) protobuf_BuildDesc_update_2dprotocol_2eproto();
  return Updates_reflection_;
}

}  // namespace Protocol
}  // namespace Lux