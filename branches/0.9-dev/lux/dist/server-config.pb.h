// Generated by the protocol buffer compiler.  DO NOT EDIT!

#ifndef PROTOBUF_server_2dconfig_2eproto__INCLUDED
#define PROTOBUF_server_2dconfig_2eproto__INCLUDED

#include <string>

#include <google/protobuf/stubs/common.h>

#if GOOGLE_PROTOBUF_VERSION < 2000003
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please update
#error your headers.
#endif
#if 2000003 < GOOGLE_PROTOBUF_MIN_PROTOC_VERSION
#error This file was generated by an older version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please
#error regenerate this file with a newer version of protoc.
#endif

#include <google/protobuf/generated_message_reflection.h>
#include <google/protobuf/repeated_field.h>
#include <google/protobuf/extension_set.h>

namespace Lux {
namespace Config {

// Internal implementation detail -- do not call these.
void  protobuf_BuildDesc_server_2dconfig_2eproto();
void protobuf_BuildDesc_server_2dconfig_2eproto_AssignGlobalDescriptors(
    ::google::protobuf::FileDescriptor* file);

class Server;
class Cluster;
class Clusters;
class ProxyMerger;
class ProxyMergers;
class Dispatcher;
class Dispatchers;
class ServerConfig;

// ===================================================================

class Server : public ::google::protobuf::Message {
 public:
  Server();
  virtual ~Server();
  
  Server(const Server& from);
  
  inline Server& operator=(const Server& from) {
    CopyFrom(from);
    return *this;
  }
  
  inline const ::google::protobuf::UnknownFieldSet& unknown_fields() const {
    return _unknown_fields_;
  }
  
  inline ::google::protobuf::UnknownFieldSet* mutable_unknown_fields() {
    return &_unknown_fields_;
  }
  
  static const ::google::protobuf::Descriptor* descriptor();
  static const Server& default_instance();
  void Swap(Server* other);
  
  // implements Message ----------------------------------------------
  
  Server* New() const;
  int GetCachedSize() const { return _cached_size_; }
  private:
  void SetCachedSize(int size) const { _cached_size_ = size; }
  public:
  
  const ::google::protobuf::Descriptor* GetDescriptor() const;
  const ::google::protobuf::Reflection* GetReflection() const;
  
  // nested types ----------------------------------------------------
  
  // accessors -------------------------------------------------------
  
  // required uint32 id = 1;
  inline bool has_id() const;
  inline void clear_id();
  inline ::google::protobuf::uint32 id() const;
  inline void set_id(::google::protobuf::uint32 value);
  
  // required string host = 2;
  inline bool has_host() const;
  inline void clear_host();
  inline const ::std::string& host() const;
  inline void set_host(const ::std::string& value);
  inline void set_host(const char* value);
  inline ::std::string* mutable_host();
  
  // required uint32 port = 3;
  inline bool has_port() const;
  inline void clear_port();
  inline ::google::protobuf::uint32 port() const;
  inline void set_port(::google::protobuf::uint32 value);
  
  // optional uint32 num_default = 4 [default = 10];
  inline bool has_num_default() const;
  inline void clear_num_default();
  inline ::google::protobuf::uint32 num_default() const;
  inline void set_num_default(::google::protobuf::uint32 value);
  
  // optional uint32 num_max = 5 [default = 30];
  inline bool has_num_max() const;
  inline void clear_num_max();
  inline ::google::protobuf::uint32 num_max() const;
  inline void set_num_max(::google::protobuf::uint32 value);
  
 private:
  ::google::protobuf::UnknownFieldSet _unknown_fields_;
  mutable int _cached_size_;
  
  ::google::protobuf::uint32 id_;
  ::std::string* host_;
  static const ::std::string _default_host_;
  ::google::protobuf::uint32 port_;
  ::google::protobuf::uint32 num_default_;
  ::google::protobuf::uint32 num_max_;
  friend void protobuf_BuildDesc_server_2dconfig_2eproto_AssignGlobalDescriptors(
      const ::google::protobuf::FileDescriptor* file);
  ::google::protobuf::uint32 _has_bits_[(5 + 31) / 32];
  
  // WHY DOES & HAVE LOWER PRECEDENCE THAN != !?
  inline bool _has_bit(int index) const {
    return (_has_bits_[index / 32] & (1u << (index % 32))) != 0;
  }
  inline void _set_bit(int index) {
    _has_bits_[index / 32] |= (1u << (index % 32));
  }
  inline void _clear_bit(int index) {
    _has_bits_[index / 32] &= ~(1u << (index % 32));
  }
  
  void InitAsDefaultInstance();
  static Server* default_instance_;
};
// -------------------------------------------------------------------

class Cluster : public ::google::protobuf::Message {
 public:
  Cluster();
  virtual ~Cluster();
  
  Cluster(const Cluster& from);
  
  inline Cluster& operator=(const Cluster& from) {
    CopyFrom(from);
    return *this;
  }
  
  inline const ::google::protobuf::UnknownFieldSet& unknown_fields() const {
    return _unknown_fields_;
  }
  
  inline ::google::protobuf::UnknownFieldSet* mutable_unknown_fields() {
    return &_unknown_fields_;
  }
  
  static const ::google::protobuf::Descriptor* descriptor();
  static const Cluster& default_instance();
  void Swap(Cluster* other);
  
  // implements Message ----------------------------------------------
  
  Cluster* New() const;
  int GetCachedSize() const { return _cached_size_; }
  private:
  void SetCachedSize(int size) const { _cached_size_ = size; }
  public:
  
  const ::google::protobuf::Descriptor* GetDescriptor() const;
  const ::google::protobuf::Reflection* GetReflection() const;
  
  // nested types ----------------------------------------------------
  
  // accessors -------------------------------------------------------
  
  // required uint32 id = 1;
  inline bool has_id() const;
  inline void clear_id();
  inline ::google::protobuf::uint32 id() const;
  inline void set_id(::google::protobuf::uint32 value);
  
  // repeated .Lux.Config.Server server = 2;
  inline int server_size() const;
  inline void clear_server();
  inline const ::google::protobuf::RepeatedPtrField< ::Lux::Config::Server >& server() const;
  inline ::google::protobuf::RepeatedPtrField< ::Lux::Config::Server >* mutable_server();
  inline const ::Lux::Config::Server& server(int index) const;
  inline ::Lux::Config::Server* mutable_server(int index);
  inline ::Lux::Config::Server* add_server();
  
 private:
  ::google::protobuf::UnknownFieldSet _unknown_fields_;
  mutable int _cached_size_;
  
  ::google::protobuf::uint32 id_;
  ::google::protobuf::RepeatedPtrField< ::Lux::Config::Server > server_;
  friend void protobuf_BuildDesc_server_2dconfig_2eproto_AssignGlobalDescriptors(
      const ::google::protobuf::FileDescriptor* file);
  ::google::protobuf::uint32 _has_bits_[(2 + 31) / 32];
  
  // WHY DOES & HAVE LOWER PRECEDENCE THAN != !?
  inline bool _has_bit(int index) const {
    return (_has_bits_[index / 32] & (1u << (index % 32))) != 0;
  }
  inline void _set_bit(int index) {
    _has_bits_[index / 32] |= (1u << (index % 32));
  }
  inline void _clear_bit(int index) {
    _has_bits_[index / 32] &= ~(1u << (index % 32));
  }
  
  void InitAsDefaultInstance();
  static Cluster* default_instance_;
};
// -------------------------------------------------------------------

class Clusters : public ::google::protobuf::Message {
 public:
  Clusters();
  virtual ~Clusters();
  
  Clusters(const Clusters& from);
  
  inline Clusters& operator=(const Clusters& from) {
    CopyFrom(from);
    return *this;
  }
  
  inline const ::google::protobuf::UnknownFieldSet& unknown_fields() const {
    return _unknown_fields_;
  }
  
  inline ::google::protobuf::UnknownFieldSet* mutable_unknown_fields() {
    return &_unknown_fields_;
  }
  
  static const ::google::protobuf::Descriptor* descriptor();
  static const Clusters& default_instance();
  void Swap(Clusters* other);
  
  // implements Message ----------------------------------------------
  
  Clusters* New() const;
  int GetCachedSize() const { return _cached_size_; }
  private:
  void SetCachedSize(int size) const { _cached_size_ = size; }
  public:
  
  const ::google::protobuf::Descriptor* GetDescriptor() const;
  const ::google::protobuf::Reflection* GetReflection() const;
  
  // nested types ----------------------------------------------------
  
  // accessors -------------------------------------------------------
  
  // repeated .Lux.Config.Cluster cluster = 1;
  inline int cluster_size() const;
  inline void clear_cluster();
  inline const ::google::protobuf::RepeatedPtrField< ::Lux::Config::Cluster >& cluster() const;
  inline ::google::protobuf::RepeatedPtrField< ::Lux::Config::Cluster >* mutable_cluster();
  inline const ::Lux::Config::Cluster& cluster(int index) const;
  inline ::Lux::Config::Cluster* mutable_cluster(int index);
  inline ::Lux::Config::Cluster* add_cluster();
  
 private:
  ::google::protobuf::UnknownFieldSet _unknown_fields_;
  mutable int _cached_size_;
  
  ::google::protobuf::RepeatedPtrField< ::Lux::Config::Cluster > cluster_;
  friend void protobuf_BuildDesc_server_2dconfig_2eproto_AssignGlobalDescriptors(
      const ::google::protobuf::FileDescriptor* file);
  ::google::protobuf::uint32 _has_bits_[(1 + 31) / 32];
  
  // WHY DOES & HAVE LOWER PRECEDENCE THAN != !?
  inline bool _has_bit(int index) const {
    return (_has_bits_[index / 32] & (1u << (index % 32))) != 0;
  }
  inline void _set_bit(int index) {
    _has_bits_[index / 32] |= (1u << (index % 32));
  }
  inline void _clear_bit(int index) {
    _has_bits_[index / 32] &= ~(1u << (index % 32));
  }
  
  void InitAsDefaultInstance();
  static Clusters* default_instance_;
};
// -------------------------------------------------------------------

class ProxyMerger : public ::google::protobuf::Message {
 public:
  ProxyMerger();
  virtual ~ProxyMerger();
  
  ProxyMerger(const ProxyMerger& from);
  
  inline ProxyMerger& operator=(const ProxyMerger& from) {
    CopyFrom(from);
    return *this;
  }
  
  inline const ::google::protobuf::UnknownFieldSet& unknown_fields() const {
    return _unknown_fields_;
  }
  
  inline ::google::protobuf::UnknownFieldSet* mutable_unknown_fields() {
    return &_unknown_fields_;
  }
  
  static const ::google::protobuf::Descriptor* descriptor();
  static const ProxyMerger& default_instance();
  void Swap(ProxyMerger* other);
  
  // implements Message ----------------------------------------------
  
  ProxyMerger* New() const;
  int GetCachedSize() const { return _cached_size_; }
  private:
  void SetCachedSize(int size) const { _cached_size_ = size; }
  public:
  
  const ::google::protobuf::Descriptor* GetDescriptor() const;
  const ::google::protobuf::Reflection* GetReflection() const;
  
  // nested types ----------------------------------------------------
  
  // accessors -------------------------------------------------------
  
  // repeated uint32 cluster_id = 1;
  inline int cluster_id_size() const;
  inline void clear_cluster_id();
  inline const ::google::protobuf::RepeatedField< ::google::protobuf::uint32 >& cluster_id() const;
  inline ::google::protobuf::RepeatedField< ::google::protobuf::uint32 >* mutable_cluster_id();
  inline ::google::protobuf::uint32 cluster_id(int index) const;
  inline void set_cluster_id(int index, ::google::protobuf::uint32 value);
  inline void add_cluster_id(::google::protobuf::uint32 value);
  
  // repeated uint32 merger_id = 2;
  inline int merger_id_size() const;
  inline void clear_merger_id();
  inline const ::google::protobuf::RepeatedField< ::google::protobuf::uint32 >& merger_id() const;
  inline ::google::protobuf::RepeatedField< ::google::protobuf::uint32 >* mutable_merger_id();
  inline ::google::protobuf::uint32 merger_id(int index) const;
  inline void set_merger_id(int index, ::google::protobuf::uint32 value);
  inline void add_merger_id(::google::protobuf::uint32 value);
  
  // required .Lux.Config.Server server = 3;
  inline bool has_server() const;
  inline void clear_server();
  inline const ::Lux::Config::Server& server() const;
  inline ::Lux::Config::Server* mutable_server();
  
 private:
  ::google::protobuf::UnknownFieldSet _unknown_fields_;
  mutable int _cached_size_;
  
  ::google::protobuf::RepeatedField< ::google::protobuf::uint32 > cluster_id_;
  ::google::protobuf::RepeatedField< ::google::protobuf::uint32 > merger_id_;
  ::Lux::Config::Server* server_;
  friend void protobuf_BuildDesc_server_2dconfig_2eproto_AssignGlobalDescriptors(
      const ::google::protobuf::FileDescriptor* file);
  ::google::protobuf::uint32 _has_bits_[(3 + 31) / 32];
  
  // WHY DOES & HAVE LOWER PRECEDENCE THAN != !?
  inline bool _has_bit(int index) const {
    return (_has_bits_[index / 32] & (1u << (index % 32))) != 0;
  }
  inline void _set_bit(int index) {
    _has_bits_[index / 32] |= (1u << (index % 32));
  }
  inline void _clear_bit(int index) {
    _has_bits_[index / 32] &= ~(1u << (index % 32));
  }
  
  void InitAsDefaultInstance();
  static ProxyMerger* default_instance_;
};
// -------------------------------------------------------------------

class ProxyMergers : public ::google::protobuf::Message {
 public:
  ProxyMergers();
  virtual ~ProxyMergers();
  
  ProxyMergers(const ProxyMergers& from);
  
  inline ProxyMergers& operator=(const ProxyMergers& from) {
    CopyFrom(from);
    return *this;
  }
  
  inline const ::google::protobuf::UnknownFieldSet& unknown_fields() const {
    return _unknown_fields_;
  }
  
  inline ::google::protobuf::UnknownFieldSet* mutable_unknown_fields() {
    return &_unknown_fields_;
  }
  
  static const ::google::protobuf::Descriptor* descriptor();
  static const ProxyMergers& default_instance();
  void Swap(ProxyMergers* other);
  
  // implements Message ----------------------------------------------
  
  ProxyMergers* New() const;
  int GetCachedSize() const { return _cached_size_; }
  private:
  void SetCachedSize(int size) const { _cached_size_ = size; }
  public:
  
  const ::google::protobuf::Descriptor* GetDescriptor() const;
  const ::google::protobuf::Reflection* GetReflection() const;
  
  // nested types ----------------------------------------------------
  
  // accessors -------------------------------------------------------
  
  // repeated .Lux.Config.ProxyMerger proxy_merger = 1;
  inline int proxy_merger_size() const;
  inline void clear_proxy_merger();
  inline const ::google::protobuf::RepeatedPtrField< ::Lux::Config::ProxyMerger >& proxy_merger() const;
  inline ::google::protobuf::RepeatedPtrField< ::Lux::Config::ProxyMerger >* mutable_proxy_merger();
  inline const ::Lux::Config::ProxyMerger& proxy_merger(int index) const;
  inline ::Lux::Config::ProxyMerger* mutable_proxy_merger(int index);
  inline ::Lux::Config::ProxyMerger* add_proxy_merger();
  
 private:
  ::google::protobuf::UnknownFieldSet _unknown_fields_;
  mutable int _cached_size_;
  
  ::google::protobuf::RepeatedPtrField< ::Lux::Config::ProxyMerger > proxy_merger_;
  friend void protobuf_BuildDesc_server_2dconfig_2eproto_AssignGlobalDescriptors(
      const ::google::protobuf::FileDescriptor* file);
  ::google::protobuf::uint32 _has_bits_[(1 + 31) / 32];
  
  // WHY DOES & HAVE LOWER PRECEDENCE THAN != !?
  inline bool _has_bit(int index) const {
    return (_has_bits_[index / 32] & (1u << (index % 32))) != 0;
  }
  inline void _set_bit(int index) {
    _has_bits_[index / 32] |= (1u << (index % 32));
  }
  inline void _clear_bit(int index) {
    _has_bits_[index / 32] &= ~(1u << (index % 32));
  }
  
  void InitAsDefaultInstance();
  static ProxyMergers* default_instance_;
};
// -------------------------------------------------------------------

class Dispatcher : public ::google::protobuf::Message {
 public:
  Dispatcher();
  virtual ~Dispatcher();
  
  Dispatcher(const Dispatcher& from);
  
  inline Dispatcher& operator=(const Dispatcher& from) {
    CopyFrom(from);
    return *this;
  }
  
  inline const ::google::protobuf::UnknownFieldSet& unknown_fields() const {
    return _unknown_fields_;
  }
  
  inline ::google::protobuf::UnknownFieldSet* mutable_unknown_fields() {
    return &_unknown_fields_;
  }
  
  static const ::google::protobuf::Descriptor* descriptor();
  static const Dispatcher& default_instance();
  void Swap(Dispatcher* other);
  
  // implements Message ----------------------------------------------
  
  Dispatcher* New() const;
  int GetCachedSize() const { return _cached_size_; }
  private:
  void SetCachedSize(int size) const { _cached_size_ = size; }
  public:
  
  const ::google::protobuf::Descriptor* GetDescriptor() const;
  const ::google::protobuf::Reflection* GetReflection() const;
  
  // nested types ----------------------------------------------------
  
  // accessors -------------------------------------------------------
  
  // optional uint32 cluster_id = 1;
  inline bool has_cluster_id() const;
  inline void clear_cluster_id();
  inline ::google::protobuf::uint32 cluster_id() const;
  inline void set_cluster_id(::google::protobuf::uint32 value);
  
  // repeated uint32 merger_id = 2;
  inline int merger_id_size() const;
  inline void clear_merger_id();
  inline const ::google::protobuf::RepeatedField< ::google::protobuf::uint32 >& merger_id() const;
  inline ::google::protobuf::RepeatedField< ::google::protobuf::uint32 >* mutable_merger_id();
  inline ::google::protobuf::uint32 merger_id(int index) const;
  inline void set_merger_id(int index, ::google::protobuf::uint32 value);
  inline void add_merger_id(::google::protobuf::uint32 value);
  
  // required .Lux.Config.Server server = 3;
  inline bool has_server() const;
  inline void clear_server();
  inline const ::Lux::Config::Server& server() const;
  inline ::Lux::Config::Server* mutable_server();
  
 private:
  ::google::protobuf::UnknownFieldSet _unknown_fields_;
  mutable int _cached_size_;
  
  ::google::protobuf::uint32 cluster_id_;
  ::google::protobuf::RepeatedField< ::google::protobuf::uint32 > merger_id_;
  ::Lux::Config::Server* server_;
  friend void protobuf_BuildDesc_server_2dconfig_2eproto_AssignGlobalDescriptors(
      const ::google::protobuf::FileDescriptor* file);
  ::google::protobuf::uint32 _has_bits_[(3 + 31) / 32];
  
  // WHY DOES & HAVE LOWER PRECEDENCE THAN != !?
  inline bool _has_bit(int index) const {
    return (_has_bits_[index / 32] & (1u << (index % 32))) != 0;
  }
  inline void _set_bit(int index) {
    _has_bits_[index / 32] |= (1u << (index % 32));
  }
  inline void _clear_bit(int index) {
    _has_bits_[index / 32] &= ~(1u << (index % 32));
  }
  
  void InitAsDefaultInstance();
  static Dispatcher* default_instance_;
};
// -------------------------------------------------------------------

class Dispatchers : public ::google::protobuf::Message {
 public:
  Dispatchers();
  virtual ~Dispatchers();
  
  Dispatchers(const Dispatchers& from);
  
  inline Dispatchers& operator=(const Dispatchers& from) {
    CopyFrom(from);
    return *this;
  }
  
  inline const ::google::protobuf::UnknownFieldSet& unknown_fields() const {
    return _unknown_fields_;
  }
  
  inline ::google::protobuf::UnknownFieldSet* mutable_unknown_fields() {
    return &_unknown_fields_;
  }
  
  static const ::google::protobuf::Descriptor* descriptor();
  static const Dispatchers& default_instance();
  void Swap(Dispatchers* other);
  
  // implements Message ----------------------------------------------
  
  Dispatchers* New() const;
  int GetCachedSize() const { return _cached_size_; }
  private:
  void SetCachedSize(int size) const { _cached_size_ = size; }
  public:
  
  const ::google::protobuf::Descriptor* GetDescriptor() const;
  const ::google::protobuf::Reflection* GetReflection() const;
  
  // nested types ----------------------------------------------------
  
  // accessors -------------------------------------------------------
  
  // repeated .Lux.Config.Dispatcher dispatcher = 1;
  inline int dispatcher_size() const;
  inline void clear_dispatcher();
  inline const ::google::protobuf::RepeatedPtrField< ::Lux::Config::Dispatcher >& dispatcher() const;
  inline ::google::protobuf::RepeatedPtrField< ::Lux::Config::Dispatcher >* mutable_dispatcher();
  inline const ::Lux::Config::Dispatcher& dispatcher(int index) const;
  inline ::Lux::Config::Dispatcher* mutable_dispatcher(int index);
  inline ::Lux::Config::Dispatcher* add_dispatcher();
  
 private:
  ::google::protobuf::UnknownFieldSet _unknown_fields_;
  mutable int _cached_size_;
  
  ::google::protobuf::RepeatedPtrField< ::Lux::Config::Dispatcher > dispatcher_;
  friend void protobuf_BuildDesc_server_2dconfig_2eproto_AssignGlobalDescriptors(
      const ::google::protobuf::FileDescriptor* file);
  ::google::protobuf::uint32 _has_bits_[(1 + 31) / 32];
  
  // WHY DOES & HAVE LOWER PRECEDENCE THAN != !?
  inline bool _has_bit(int index) const {
    return (_has_bits_[index / 32] & (1u << (index % 32))) != 0;
  }
  inline void _set_bit(int index) {
    _has_bits_[index / 32] |= (1u << (index % 32));
  }
  inline void _clear_bit(int index) {
    _has_bits_[index / 32] &= ~(1u << (index % 32));
  }
  
  void InitAsDefaultInstance();
  static Dispatchers* default_instance_;
};
// -------------------------------------------------------------------

class ServerConfig : public ::google::protobuf::Message {
 public:
  ServerConfig();
  virtual ~ServerConfig();
  
  ServerConfig(const ServerConfig& from);
  
  inline ServerConfig& operator=(const ServerConfig& from) {
    CopyFrom(from);
    return *this;
  }
  
  inline const ::google::protobuf::UnknownFieldSet& unknown_fields() const {
    return _unknown_fields_;
  }
  
  inline ::google::protobuf::UnknownFieldSet* mutable_unknown_fields() {
    return &_unknown_fields_;
  }
  
  static const ::google::protobuf::Descriptor* descriptor();
  static const ServerConfig& default_instance();
  void Swap(ServerConfig* other);
  
  // implements Message ----------------------------------------------
  
  ServerConfig* New() const;
  int GetCachedSize() const { return _cached_size_; }
  private:
  void SetCachedSize(int size) const { _cached_size_ = size; }
  public:
  
  const ::google::protobuf::Descriptor* GetDescriptor() const;
  const ::google::protobuf::Reflection* GetReflection() const;
  
  // nested types ----------------------------------------------------
  
  // accessors -------------------------------------------------------
  
  // required .Lux.Config.Clusters clusters = 1;
  inline bool has_clusters() const;
  inline void clear_clusters();
  inline const ::Lux::Config::Clusters& clusters() const;
  inline ::Lux::Config::Clusters* mutable_clusters();
  
  // optional .Lux.Config.Dispatchers dispatchers = 2;
  inline bool has_dispatchers() const;
  inline void clear_dispatchers();
  inline const ::Lux::Config::Dispatchers& dispatchers() const;
  inline ::Lux::Config::Dispatchers* mutable_dispatchers();
  
  // optional .Lux.Config.ProxyMergers proxy_mergers = 3;
  inline bool has_proxy_mergers() const;
  inline void clear_proxy_mergers();
  inline const ::Lux::Config::ProxyMergers& proxy_mergers() const;
  inline ::Lux::Config::ProxyMergers* mutable_proxy_mergers();
  
 private:
  ::google::protobuf::UnknownFieldSet _unknown_fields_;
  mutable int _cached_size_;
  
  ::Lux::Config::Clusters* clusters_;
  ::Lux::Config::Dispatchers* dispatchers_;
  ::Lux::Config::ProxyMergers* proxy_mergers_;
  friend void protobuf_BuildDesc_server_2dconfig_2eproto_AssignGlobalDescriptors(
      const ::google::protobuf::FileDescriptor* file);
  ::google::protobuf::uint32 _has_bits_[(3 + 31) / 32];
  
  // WHY DOES & HAVE LOWER PRECEDENCE THAN != !?
  inline bool _has_bit(int index) const {
    return (_has_bits_[index / 32] & (1u << (index % 32))) != 0;
  }
  inline void _set_bit(int index) {
    _has_bits_[index / 32] |= (1u << (index % 32));
  }
  inline void _clear_bit(int index) {
    _has_bits_[index / 32] &= ~(1u << (index % 32));
  }
  
  void InitAsDefaultInstance();
  static ServerConfig* default_instance_;
};
// ===================================================================


// ===================================================================


// ===================================================================

// Server

// required uint32 id = 1;
inline bool Server::has_id() const {
  return _has_bit(0);
}
inline void Server::clear_id() {
  id_ = 0u;
  _clear_bit(0);
}
inline ::google::protobuf::uint32 Server::id() const {
  return id_;
}
inline void Server::set_id(::google::protobuf::uint32 value) {
  _set_bit(0);
  id_ = value;
}

// required string host = 2;
inline bool Server::has_host() const {
  return _has_bit(1);
}
inline void Server::clear_host() {
  if (host_ != &_default_host_) {
    host_->clear();
  }
  _clear_bit(1);
}
inline const ::std::string& Server::host() const {
  return *host_;
}
inline void Server::set_host(const ::std::string& value) {
  _set_bit(1);
  if (host_ == &_default_host_) {
    host_ = new ::std::string;
  }
  host_->assign(value);
}
inline void Server::set_host(const char* value) {
  _set_bit(1);
  if (host_ == &_default_host_) {
    host_ = new ::std::string;
  }
  host_->assign(value);
}
inline ::std::string* Server::mutable_host() {
  _set_bit(1);
  if (host_ == &_default_host_) {
    host_ = new ::std::string;
  }
  return host_;
}

// required uint32 port = 3;
inline bool Server::has_port() const {
  return _has_bit(2);
}
inline void Server::clear_port() {
  port_ = 0u;
  _clear_bit(2);
}
inline ::google::protobuf::uint32 Server::port() const {
  return port_;
}
inline void Server::set_port(::google::protobuf::uint32 value) {
  _set_bit(2);
  port_ = value;
}

// optional uint32 num_default = 4 [default = 10];
inline bool Server::has_num_default() const {
  return _has_bit(3);
}
inline void Server::clear_num_default() {
  num_default_ = 10u;
  _clear_bit(3);
}
inline ::google::protobuf::uint32 Server::num_default() const {
  return num_default_;
}
inline void Server::set_num_default(::google::protobuf::uint32 value) {
  _set_bit(3);
  num_default_ = value;
}

// optional uint32 num_max = 5 [default = 30];
inline bool Server::has_num_max() const {
  return _has_bit(4);
}
inline void Server::clear_num_max() {
  num_max_ = 30u;
  _clear_bit(4);
}
inline ::google::protobuf::uint32 Server::num_max() const {
  return num_max_;
}
inline void Server::set_num_max(::google::protobuf::uint32 value) {
  _set_bit(4);
  num_max_ = value;
}

// -------------------------------------------------------------------

// Cluster

// required uint32 id = 1;
inline bool Cluster::has_id() const {
  return _has_bit(0);
}
inline void Cluster::clear_id() {
  id_ = 0u;
  _clear_bit(0);
}
inline ::google::protobuf::uint32 Cluster::id() const {
  return id_;
}
inline void Cluster::set_id(::google::protobuf::uint32 value) {
  _set_bit(0);
  id_ = value;
}

// repeated .Lux.Config.Server server = 2;
inline int Cluster::server_size() const {
  return server_.size();
}
inline void Cluster::clear_server() {
  server_.Clear();
}
inline const ::google::protobuf::RepeatedPtrField< ::Lux::Config::Server >&
Cluster::server() const {
  return server_;
}
inline ::google::protobuf::RepeatedPtrField< ::Lux::Config::Server >*
Cluster::mutable_server() {
  return &server_;
}
inline const ::Lux::Config::Server& Cluster::server(int index) const {
  return server_.Get(index);
}
inline ::Lux::Config::Server* Cluster::mutable_server(int index) {
  return server_.Mutable(index);
}
inline ::Lux::Config::Server* Cluster::add_server() {
  return server_.Add();
}

// -------------------------------------------------------------------

// Clusters

// repeated .Lux.Config.Cluster cluster = 1;
inline int Clusters::cluster_size() const {
  return cluster_.size();
}
inline void Clusters::clear_cluster() {
  cluster_.Clear();
}
inline const ::google::protobuf::RepeatedPtrField< ::Lux::Config::Cluster >&
Clusters::cluster() const {
  return cluster_;
}
inline ::google::protobuf::RepeatedPtrField< ::Lux::Config::Cluster >*
Clusters::mutable_cluster() {
  return &cluster_;
}
inline const ::Lux::Config::Cluster& Clusters::cluster(int index) const {
  return cluster_.Get(index);
}
inline ::Lux::Config::Cluster* Clusters::mutable_cluster(int index) {
  return cluster_.Mutable(index);
}
inline ::Lux::Config::Cluster* Clusters::add_cluster() {
  return cluster_.Add();
}

// -------------------------------------------------------------------

// ProxyMerger

// repeated uint32 cluster_id = 1;
inline int ProxyMerger::cluster_id_size() const {
  return cluster_id_.size();
}
inline void ProxyMerger::clear_cluster_id() {
  cluster_id_.Clear();
}
inline const ::google::protobuf::RepeatedField< ::google::protobuf::uint32 >&
ProxyMerger::cluster_id() const {
  return cluster_id_;
}
inline ::google::protobuf::RepeatedField< ::google::protobuf::uint32 >*
ProxyMerger::mutable_cluster_id() {
  return &cluster_id_;
}
inline ::google::protobuf::uint32 ProxyMerger::cluster_id(int index) const {
  return cluster_id_.Get(index);
}
inline void ProxyMerger::set_cluster_id(int index, ::google::protobuf::uint32 value) {
  cluster_id_.Set(index, value);
}
inline void ProxyMerger::add_cluster_id(::google::protobuf::uint32 value) {
  cluster_id_.Add(value);
}

// repeated uint32 merger_id = 2;
inline int ProxyMerger::merger_id_size() const {
  return merger_id_.size();
}
inline void ProxyMerger::clear_merger_id() {
  merger_id_.Clear();
}
inline const ::google::protobuf::RepeatedField< ::google::protobuf::uint32 >&
ProxyMerger::merger_id() const {
  return merger_id_;
}
inline ::google::protobuf::RepeatedField< ::google::protobuf::uint32 >*
ProxyMerger::mutable_merger_id() {
  return &merger_id_;
}
inline ::google::protobuf::uint32 ProxyMerger::merger_id(int index) const {
  return merger_id_.Get(index);
}
inline void ProxyMerger::set_merger_id(int index, ::google::protobuf::uint32 value) {
  merger_id_.Set(index, value);
}
inline void ProxyMerger::add_merger_id(::google::protobuf::uint32 value) {
  merger_id_.Add(value);
}

// required .Lux.Config.Server server = 3;
inline bool ProxyMerger::has_server() const {
  return _has_bit(2);
}
inline void ProxyMerger::clear_server() {
  if (server_ != NULL) server_->::Lux::Config::Server::Clear();
  _clear_bit(2);
}
inline const ::Lux::Config::Server& ProxyMerger::server() const {
  return server_ != NULL ? *server_ : *default_instance_->server_;
}
inline ::Lux::Config::Server* ProxyMerger::mutable_server() {
  _set_bit(2);
  if (server_ == NULL) server_ = new ::Lux::Config::Server;
  return server_;
}

// -------------------------------------------------------------------

// ProxyMergers

// repeated .Lux.Config.ProxyMerger proxy_merger = 1;
inline int ProxyMergers::proxy_merger_size() const {
  return proxy_merger_.size();
}
inline void ProxyMergers::clear_proxy_merger() {
  proxy_merger_.Clear();
}
inline const ::google::protobuf::RepeatedPtrField< ::Lux::Config::ProxyMerger >&
ProxyMergers::proxy_merger() const {
  return proxy_merger_;
}
inline ::google::protobuf::RepeatedPtrField< ::Lux::Config::ProxyMerger >*
ProxyMergers::mutable_proxy_merger() {
  return &proxy_merger_;
}
inline const ::Lux::Config::ProxyMerger& ProxyMergers::proxy_merger(int index) const {
  return proxy_merger_.Get(index);
}
inline ::Lux::Config::ProxyMerger* ProxyMergers::mutable_proxy_merger(int index) {
  return proxy_merger_.Mutable(index);
}
inline ::Lux::Config::ProxyMerger* ProxyMergers::add_proxy_merger() {
  return proxy_merger_.Add();
}

// -------------------------------------------------------------------

// Dispatcher

// optional uint32 cluster_id = 1;
inline bool Dispatcher::has_cluster_id() const {
  return _has_bit(0);
}
inline void Dispatcher::clear_cluster_id() {
  cluster_id_ = 0u;
  _clear_bit(0);
}
inline ::google::protobuf::uint32 Dispatcher::cluster_id() const {
  return cluster_id_;
}
inline void Dispatcher::set_cluster_id(::google::protobuf::uint32 value) {
  _set_bit(0);
  cluster_id_ = value;
}

// repeated uint32 merger_id = 2;
inline int Dispatcher::merger_id_size() const {
  return merger_id_.size();
}
inline void Dispatcher::clear_merger_id() {
  merger_id_.Clear();
}
inline const ::google::protobuf::RepeatedField< ::google::protobuf::uint32 >&
Dispatcher::merger_id() const {
  return merger_id_;
}
inline ::google::protobuf::RepeatedField< ::google::protobuf::uint32 >*
Dispatcher::mutable_merger_id() {
  return &merger_id_;
}
inline ::google::protobuf::uint32 Dispatcher::merger_id(int index) const {
  return merger_id_.Get(index);
}
inline void Dispatcher::set_merger_id(int index, ::google::protobuf::uint32 value) {
  merger_id_.Set(index, value);
}
inline void Dispatcher::add_merger_id(::google::protobuf::uint32 value) {
  merger_id_.Add(value);
}

// required .Lux.Config.Server server = 3;
inline bool Dispatcher::has_server() const {
  return _has_bit(2);
}
inline void Dispatcher::clear_server() {
  if (server_ != NULL) server_->::Lux::Config::Server::Clear();
  _clear_bit(2);
}
inline const ::Lux::Config::Server& Dispatcher::server() const {
  return server_ != NULL ? *server_ : *default_instance_->server_;
}
inline ::Lux::Config::Server* Dispatcher::mutable_server() {
  _set_bit(2);
  if (server_ == NULL) server_ = new ::Lux::Config::Server;
  return server_;
}

// -------------------------------------------------------------------

// Dispatchers

// repeated .Lux.Config.Dispatcher dispatcher = 1;
inline int Dispatchers::dispatcher_size() const {
  return dispatcher_.size();
}
inline void Dispatchers::clear_dispatcher() {
  dispatcher_.Clear();
}
inline const ::google::protobuf::RepeatedPtrField< ::Lux::Config::Dispatcher >&
Dispatchers::dispatcher() const {
  return dispatcher_;
}
inline ::google::protobuf::RepeatedPtrField< ::Lux::Config::Dispatcher >*
Dispatchers::mutable_dispatcher() {
  return &dispatcher_;
}
inline const ::Lux::Config::Dispatcher& Dispatchers::dispatcher(int index) const {
  return dispatcher_.Get(index);
}
inline ::Lux::Config::Dispatcher* Dispatchers::mutable_dispatcher(int index) {
  return dispatcher_.Mutable(index);
}
inline ::Lux::Config::Dispatcher* Dispatchers::add_dispatcher() {
  return dispatcher_.Add();
}

// -------------------------------------------------------------------

// ServerConfig

// required .Lux.Config.Clusters clusters = 1;
inline bool ServerConfig::has_clusters() const {
  return _has_bit(0);
}
inline void ServerConfig::clear_clusters() {
  if (clusters_ != NULL) clusters_->::Lux::Config::Clusters::Clear();
  _clear_bit(0);
}
inline const ::Lux::Config::Clusters& ServerConfig::clusters() const {
  return clusters_ != NULL ? *clusters_ : *default_instance_->clusters_;
}
inline ::Lux::Config::Clusters* ServerConfig::mutable_clusters() {
  _set_bit(0);
  if (clusters_ == NULL) clusters_ = new ::Lux::Config::Clusters;
  return clusters_;
}

// optional .Lux.Config.Dispatchers dispatchers = 2;
inline bool ServerConfig::has_dispatchers() const {
  return _has_bit(1);
}
inline void ServerConfig::clear_dispatchers() {
  if (dispatchers_ != NULL) dispatchers_->::Lux::Config::Dispatchers::Clear();
  _clear_bit(1);
}
inline const ::Lux::Config::Dispatchers& ServerConfig::dispatchers() const {
  return dispatchers_ != NULL ? *dispatchers_ : *default_instance_->dispatchers_;
}
inline ::Lux::Config::Dispatchers* ServerConfig::mutable_dispatchers() {
  _set_bit(1);
  if (dispatchers_ == NULL) dispatchers_ = new ::Lux::Config::Dispatchers;
  return dispatchers_;
}

// optional .Lux.Config.ProxyMergers proxy_mergers = 3;
inline bool ServerConfig::has_proxy_mergers() const {
  return _has_bit(2);
}
inline void ServerConfig::clear_proxy_mergers() {
  if (proxy_mergers_ != NULL) proxy_mergers_->::Lux::Config::ProxyMergers::Clear();
  _clear_bit(2);
}
inline const ::Lux::Config::ProxyMergers& ServerConfig::proxy_mergers() const {
  return proxy_mergers_ != NULL ? *proxy_mergers_ : *default_instance_->proxy_mergers_;
}
inline ::Lux::Config::ProxyMergers* ServerConfig::mutable_proxy_mergers() {
  _set_bit(2);
  if (proxy_mergers_ == NULL) proxy_mergers_ = new ::Lux::Config::ProxyMergers;
  return proxy_mergers_;
}


}  // namespace Config
}  // namespace Lux
#endif  // PROTOBUF_server_2dconfig_2eproto__INCLUDED