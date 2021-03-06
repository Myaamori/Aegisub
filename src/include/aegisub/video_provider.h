// Copyright (c) 2006-2008, Rodrigo Braz Monteiro, Fredrik Mellbin
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
//   * Redistributions of source code must retain the above copyright notice,
//     this list of conditions and the following disclaimer.
//   * Redistributions in binary form must reproduce the above copyright notice,
//     this list of conditions and the following disclaimer in the documentation
//     and/or other materials provided with the distribution.
//   * Neither the name of the Aegisub Group nor the names of its contributors
//     may be used to endorse or promote products derived from this software
//     without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
//
// Aegisub Project http://www.aegisub.org/

/// @file video_provider.h
/// @brief Declaration of base-class for video providers
/// @ingroup main_headers video_input
///

#pragma once

#include <libaegisub/exception.h>
#include <libaegisub/vfr.h>

#include <string>

struct VideoFrame;

class VideoProvider {
public:
	virtual ~VideoProvider() = default;

	/// Override this method to actually get frames
	virtual void GetFrame(int n, VideoFrame &frame)=0;

	/// Set the YCbCr matrix to the specified one
	///
	/// Providers are free to disregard this, and should if the requested
	/// matrix makes no sense or the input isn't YCbCr.
	virtual void SetColorSpace(std::string const& matrix)=0;

	// Override the following methods to get video information:
	virtual int GetFrameCount() const=0;			///< Get total number of frames
	virtual int GetWidth() const=0;					///< Returns the video width in pixels
	virtual int GetHeight() const=0;				///< Returns the video height in pixels
	virtual double GetDAR() const=0;				///< Returns the video display aspect ratio
	virtual agi::vfr::Framerate GetFPS() const=0;	///< Get frame rate
	virtual std::vector<int> GetKeyFrames() const=0;///< Returns list of keyframes

	/// Get the source colorspace of the video before it was converted to RGB
	/// @return A string describing the source colorspace or "None" if it is
	///         unknown or meaningless
	virtual std::string GetColorSpace() const = 0;
	virtual std::string GetRealColorSpace() const { return GetColorSpace(); }

	/// @brief Use this to set any post-loading warnings, such as "being loaded with unreliable seeking"
	virtual std::string GetWarning() const { return ""; }

	/// @brief Name of decoder, e.g. "Avisynth/FFMpegSource"
	virtual std::string GetDecoderName() const = 0;

	/// @brief Does this provider want Aegisub to cache video frames?
	/// @return Returns true if caching is desired, false otherwise.
	virtual bool WantsCaching() const { return false; }

	/// Should the video properties in the script be set to this video's property if they already have values?
	virtual bool ShouldSetVideoProperties() const { return true; }

	/// Does the file which this provider is reading have an audio track?
	virtual bool HasAudio() const { return false; }
};

DEFINE_EXCEPTION(VideoProviderError, agi::Exception);
/// File could be opened, but is not a supported format
DEFINE_EXCEPTION(VideoNotSupported, VideoProviderError);
/// File appears to be a supported format, but could not be opened
DEFINE_EXCEPTION(VideoOpenError, VideoProviderError);

/// Error of some sort occurred while decoding a frame
DEFINE_EXCEPTION(VideoDecodeError, VideoProviderError);
