create thread

```c++
// create webrtc signaling thread
bool Conductor::InitializePeerConnection(){
  if (!signaling_thread_.get()) {
    signaling_thread_ = rtc::Thread::CreateWithSocketServer();
    signaling_thread_->Start();
  }

// create peer_connection factory
  peer_connection_factory_ = webrtc::CreatePeerConnectionFactory(
      nullptr /* network_thread */, nullptr /* worker_thread */,
      signaling_thread_.get(), nullptr /* default_adm */,
      webrtc::CreateBuiltinAudioEncoderFactory(),
      webrtc::CreateBuiltinAudioDecoderFactory(),
      std::make_unique<webrtc::VideoEncoderFactoryTemplate<
          webrtc::LibvpxVp8EncoderTemplateAdapter,
          webrtc::LibvpxVp9EncoderTemplateAdapter,
          webrtc::OpenH264EncoderTemplateAdapter,
          webrtc::LibaomAv1EncoderTemplateAdapter>>(),
      std::make_unique<webrtc::VideoDecoderFactoryTemplate<
          webrtc::LibvpxVp8DecoderTemplateAdapter,
          webrtc::LibvpxVp9DecoderTemplateAdapter,
          webrtc::OpenH264DecoderTemplateAdapter,
          webrtc::Dav1dDecoderTemplateAdapter>>(),
      nullptr /* audio_mixer */, nullptr /* audio_processing */);
}
```