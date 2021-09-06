use std::{sync::mpsc::sync_channel, thread};

use glib::{Continue, MainLoop, ObjectExt};
use gstreamer::{ClockTime, Element, GenericFormattedValue, SeekFlags, State, format::Percent, prelude::ElementExtManual, traits::ElementExt};


/// PAT Audio Technician (PAT)
pub struct PAT {
    main_loop: MainLoop,
    playbin: Element,
}

impl PAT {
    /// Creates a new PAT instance and initializes PAT if it has already been initialized.
    ///
    /// # Examples
    /// ```
    /// rustpat::PAT::new().unwrap();
    /// ```
    pub fn new() -> Result<PAT, PATError> {
        gstreamer::init().map_err(|_e| PATError::InitializationError)?;
        let main_loop = glib::MainLoop::new(None, false);

        let main_loop_clone = main_loop.clone();
        thread::spawn(move || main_loop_clone.run());

        let playbin = gstreamer::ElementFactory::make("playbin", Some("playbin")).map_err(|_e| PATError::InitializationError)?;
        
        playbin.set_state(State::Ready).map_err(|_e| PATError::InitializationError)?;

        Ok(PAT{
            main_loop,
            playbin,
        })
    }

    /// Play an audio file.
    ///
    /// The audio file can be a local file or remote file (http:// or https://).
    ///
    /// # Examples
    /// ```
    /// let pat = rustpat::PAT::new().unwrap();
    /// let test_audio = format!("{}/tests/test.ogg", env!("CARGO_MANIFEST_DIR"));
    /// pat.play(&test_audio).unwrap();
    /// ```
    pub fn play(&self, pat_audio_path: &str) -> Result<(), PATError> {
        let pat_audio_path = match pat_audio_path.contains("://") {
            true => pat_audio_path.to_string(),
            false => format!("file://{}", pat_audio_path),
        };

        self.playbin.set_property("uri", pat_audio_path).map_err(|_e| PATError::FileOpenError)?;

        let bus = self.playbin.bus().unwrap();

        let (sender, receiver) = sync_channel::<Result<(), PATError>>(1);

        bus.add_watch(move |_bus, message| {
            match message.view() {
                gstreamer::MessageView::StateChanged(state_changed) => {
                    match state_changed.current() == State::Null {
                        true => {
                            sender.send(Ok(())).unwrap();
                            Continue(false)
                        },
                        false => {
                            Continue(true)
                        }
                    }

                },
                gstreamer::MessageView::Eos(_) => {
                    sender.send(Ok(())).unwrap();
                    Continue(false)
                },
                gstreamer::MessageView::Error(error) => {
                    println!("{:?}", error.debug());
                    sender.send(Err(PATError::DecodeError)).unwrap();
                    Continue(false)
                }
              _ => {
                  println!("{:?}", message);
                  Continue(true)
              }
            }
        }).map_err(|_e| PATError::FileOpenError)?;

        self.playbin.set_state(State::Playing).map_err(|_e| PATError::DecodeError)?;

        let result = match receiver.recv() {
            Ok(result) => result,
            Err(_) => Err(PATError::UnknownError),
        };
        
        result
    }

    /// Skip playback of the current audio file.
    ///
    /// This method does nothing if there is not an audio file playing.
    ///
    /// # Examples
    /// ```
    /// let pat = rustpat::PAT::new().unwrap();
    /// pat.skip().unwrap();
    /// ```
    pub fn skip(&self) -> Result<(), PATError> {
        self.playbin.set_state(State::Ready).map_err(|_e| PATError::DecodeError)?;

        Ok(())
    }

    /// Pause audio playback.
    ///
    /// This method does nothing if audio playback is already paused.
    ///
    /// # Examples
    /// ```
    /// let pat = rustpat::PAT::new().unwrap();
    /// pat.pause().unwrap();
    /// ```
    pub fn pause(&self) -> Result<(), PATError> {
        if self.is_state(State::Playing) {
            self.playbin.set_state(State::Paused).map_err(|_e| PATError::DecodeError)?;
        }

        Ok(())
    }

    /// Resume audio playback.
    ///
    /// This method does nothing if audio playback is already resumed.
    ///
    /// # Examples
    /// ```
    /// let pat = rustpat::PAT::new().unwrap();
    /// pat.resume().unwrap();
    /// ```
    pub fn resume(&self) -> Result<(), PATError> {
        if self.is_state(State::Paused) {
            self.playbin.set_state(State::Playing).map_err(|_e| PATError::DecodeError)?;
        }

        Ok(())
    }

    fn is_state(&self, desired_state: State) -> bool {
        let (_result, current_state, _pending) = self.playbin.state(ClockTime::from_seconds(0));

        current_state == desired_state
    }
}

impl Drop for PAT {
    fn drop(&mut self) {
        self.playbin.set_state(State::Null).unwrap();
        self.main_loop.quit();
    }
}

#[derive(Clone, Copy, Debug, Eq, Hash, Ord, PartialEq, PartialOrd)]
pub enum PATError {
    InitializationError,
    AudioDeviceError,
    DemuxError,
    DecodeError,
    FileOpenError,
    InterruptedError,
    MemoryError,
    ResampleError,
    TerminatedError,
    UnknownError,
}
