﻿/* ==================================================================================================== */
/* FMOD Ex - codec development header file. Copyright (c), Firelight Technologies Pty, Ltd. 2004-2014.  */
/*                                                                                                      */
/* Use this header if you are wanting to develop your own file format plugin to use with                */
/* FMOD's codec system.  With this header you can make your own fileformat plugin that FMOD             */
/* can register and use.  See the documentation and examples on how to make a working plugin.           */
/*                                                                                                      */
/* ==================================================================================================== */

#ifndef _FMOD_CODEC_H
#define _FMOD_CODEC_H

typedef struct FMOD_CODEC_STATE FMOD_CODEC_STATE;
typedef struct FMOD_CODEC_WAVEFORMAT FMOD_CODEC_WAVEFORMAT;

/*
    Codec callbacks
*/ 
typedef FMOD_RESULT (F_CALLBACK *FMOD_CODEC_OPENCALLBACK)        (FMOD_CODEC_STATE *codec_state, FMOD_MODE usermode, FMOD_CREATESOUNDEXINFO *userexinfo);
typedef FMOD_RESULT (F_CALLBACK *FMOD_CODEC_CLOSECALLBACK)       (FMOD_CODEC_STATE *codec_state);
typedef FMOD_RESULT (F_CALLBACK *FMOD_CODEC_READCALLBACK)        (FMOD_CODEC_STATE *codec_state, void *buffer, unsigned int sizebytes, unsigned int *bytesread);
typedef FMOD_RESULT (F_CALLBACK *FMOD_CODEC_GETLENGTHCALLBACK)   (FMOD_CODEC_STATE *codec_state, unsigned int *length, FMOD_TIMEUNIT lengthtype);
typedef FMOD_RESULT (F_CALLBACK *FMOD_CODEC_SETPOSITIONCALLBACK) (FMOD_CODEC_STATE *codec_state, int subsound, unsigned int position, FMOD_TIMEUNIT postype);
typedef FMOD_RESULT (F_CALLBACK *FMOD_CODEC_GETPOSITIONCALLBACK) (FMOD_CODEC_STATE *codec_state, unsigned int *position, FMOD_TIMEUNIT postype);
typedef FMOD_RESULT (F_CALLBACK *FMOD_CODEC_SOUNDCREATECALLBACK) (FMOD_CODEC_STATE *codec_state, int subsound, FMOD_SOUND *sound);
typedef FMOD_RESULT (F_CALLBACK *FMOD_CODEC_METADATACALLBACK)    (FMOD_CODEC_STATE *codec_state, FMOD_TAGTYPE tagtype, char *name, void *data, unsigned int datalen, FMOD_TAGDATATYPE datatype, int unique);
typedef FMOD_RESULT (F_CALLBACK *FMOD_CODEC_GETWAVEFORMAT)       (FMOD_CODEC_STATE *codec_state, int index, FMOD_CODEC_WAVEFORMAT *waveformat);


/*
[STRUCTURE] 
[
    [DESCRIPTION]
    When creating a codec, declare one of these and provide the relevant callbacks and name for FMOD to use when it opens and reads a file.

    [REMARKS]
    Members marked with [in] mean the variable can be written to.  The user can set the value.
    Members marked with [out] mean the variable is modified by FMOD and is for reading purposes only.  Do not change this value.

    [PLATFORMS]
    Win32, Win64, Linux, Linux64, Macintosh, Xbox360, PlayStation Portable, PlayStation 3, Wii, iPhone, 3GS, NGP, Android

    [SEE_ALSO]
    FMOD_CODEC_STATE
]
*/
typedef struct FMOD_CODEC_DESCRIPTION
{
    const char                     *name;            /* [in] Name of the codec. */
    unsigned int                    version;         /* [in] Plugin writer's version number. */
    int                             defaultasstream; /* [in] Tells FMOD to open the file as a stream when calling System::createSound, and not a static sample.  Should normally be 0 (FALSE), because generally the user wants to decode the file into memory when using System::createSound.   Mainly used for formats that decode for a very long time, or could use large amounts of memory when decoded.  Usually sequenced formats such as mod/s3m/xm/it/midi fall into this category.   It is mainly to stop users that don't know what they're doing from getting FMOD_ERR_MEMORY returned from createSound when they should have in fact called System::createStream or used FMOD_CREATESTREAM in System::createSound. */
    FMOD_TIMEUNIT                   timeunits;       /* [in] When setposition codec is called, only these time formats will be passed to the codec. Use bitwise OR to accumulate different types. */
    FMOD_CODEC_OPENCALLBACK         open;            /* [in] Open callback for the codec for when FMOD tries to open a sound using this codec. */
    FMOD_CODEC_CLOSECALLBACK        close;           /* [in] Close callback for the codec for when FMOD tries to close a sound using this codec.  */
    FMOD_CODEC_READCALLBACK         read;            /* [in] Read callback for the codec for when FMOD tries to read some data from the file to the destination format (specified in the open callback). */
    FMOD_CODEC_GETLENGTHCALLBACK    getlength;       /* [in] Callback to return the length of the song in whatever format required when Sound::getLength is called. */
    FMOD_CODEC_SETPOSITIONCALLBACK  setposition;     /* [in] Seek callback for the codec for when FMOD tries to seek within the file with Channel::setPosition. */
    FMOD_CODEC_GETPOSITIONCALLBACK  getposition;     /* [in] Tell callback for the codec for when FMOD tries to get the current position within the with Channel::getPosition. */
    FMOD_CODEC_SOUNDCREATECALLBACK  soundcreate;     /* [in] Sound creation callback for the codec when FMOD finishes creating the sound.  (So the codec can set more parameters for the related created sound, ie loop points/mode or 3D attributes etc). */
    FMOD_CODEC_GETWAVEFORMAT        getwaveformat;   /* [in] Callback to tell FMOD about the waveformat of a particular subsound.  This is to save memory, rather than saving 1000 FMOD_CODEC_WAVEFORMAT structures in the codec, the codec might have a more optimal way of storing this information. */
} FMOD_CODEC_DESCRIPTION;


/*
[STRUCTURE] 
[
    [DESCRIPTION]
    Set these values marked 'in' to tell fmod what sort of sound to create.
    The format, channels and frequency tell FMOD what sort of hardware buffer to create when you initialize your code.  So if you wrote an MP3 codec that decoded to stereo 16bit integer PCM, you would specify FMOD_SOUND_FORMAT_PCM16, and channels would be equal to 2.
    Members marked as 'out' are set by fmod.  Do not modify these.  Simply specify 0 for these values when declaring the structure, FMOD will fill in the values for you after creation with the correct function pointers.

    [REMARKS]
    Members marked with [in] mean the variable can be written to.  The user can set the value.
    Members marked with [out] mean the variable is modified by FMOD and is for reading purposes only.  Do not change this value.
    
    An FMOD file might be from disk, memory or network, however the file may be opened by the user.
    
    'numsubsounds' should be 0 if the file is a normal single sound stream or sound.  Examples of this would be .WAV, .WMA, .MP3, .AIFF.
    'numsubsounds' should be 1+ if the file is a container format, and does not contain wav data itself.  Examples of these types would be CDDA (multiple CD tracks), FSB (contains multiple sounds), MIDI/MOD/S3M/XM/IT (contain instruments).
    The arrays of format, channel, frequency, length and blockalign should point to arrays of information based on how many subsounds are in the format.  If the number of subsounds is 0 then it should point to 1 of each attribute, the same as if the number of subsounds was 1.  If subsounds was 100 for example, each pointer should point to an array of 100 of each attribute.
    When a sound has 1 or more subsounds, you must play the individual sounds specified by first obtaining the subsound with Sound::getSubSound.
    
    [PLATFORMS]
    Win32, Win64, Linux, Linux64, Macintosh, Xbox360, PlayStation Portable, PlayStation 3, Wii, iPhone, 3GS, NGP, Android

    [SEE_ALSO]
    FMOD_SOUND_FORMAT
    FMOD_FILE_READCALLBACK      
    FMOD_FILE_SEEKCALLBACK      
    FMOD_CODEC_METADATACALLBACK
    Sound::getSubSound
    Sound::getNumSubSounds
]
*/
struct FMOD_CODEC_WAVEFORMAT
{
    char               name[256];     /* [in] Name of sound.*/
    FMOD_SOUND_FORMAT  format;        /* [in] Format for (decompressed) codec output, ie FMOD_SOUND_FORMAT_PCM8, FMOD_SOUND_FORMAT_PCM16.*/
    int                channels;      /* [in] Number of channels used by codec, ie mono = 1, stereo = 2. */
    int                frequency;     /* [in] Default frequency in hz of the codec, ie 44100. */
    unsigned int       lengthbytes;   /* [in] Length in bytes of the source data. */
    unsigned int       lengthpcm;     /* [in] Length in decompressed, PCM samples of the file, ie length in seconds * frequency.  Used for Sound::getLength and for memory allocation of static decompressed sample data. */
    int                blockalign;    /* [in] Blockalign in decompressed, PCM samples of the optimal decode chunk size for this format.  The codec read callback will be called in multiples of this value. */
    int                loopstart;     /* [in] Loopstart in decompressed, PCM samples of file. */
    int                loopend;       /* [in] Loopend in decompressed, PCM samples of file. */
    FMOD_MODE          mode;          /* [in] Mode to determine whether the sound should by default load as looping, non looping, 2d or 3d. */
    unsigned int       channelmask;   /* [in] Microsoft speaker channel mask, as defined for WAVEFORMATEXTENSIBLE and is found in ksmedia.h.  Leave at 0 to play in natural speaker order. */
};


/*
[STRUCTURE] 
[
    [DESCRIPTION]
    Codec plugin structure that is passed into each callback.
    
    Set these numsubsounds and waveformat members when called in FMOD_CODEC_OPENCALLBACK to tell fmod what sort of sound to create.
    
    The format, channels and frequency tell FMOD what sort of hardware buffer to create when you initialize your code.  So if you wrote an MP3 codec that decoded to stereo 16bit integer PCM, you would specify FMOD_SOUND_FORMAT_PCM16, and channels would be equal to 2.

    [REMARKS]
    Members marked with [in] mean the variable can be written to.  The user can set the value.
    Members marked with [out] mean the variable is modified by FMOD and is for reading purposes only.  Do not change this value.
    
    An FMOD file might be from disk, memory or internet, however the file may be opened by the user.
    
    'numsubsounds' should be 0 if the file is a normal single sound stream or sound.  Examples of this would be .WAV, .WMA, .MP3, .AIFF.
    'numsubsounds' should be 1+ if the file is a container format, and does not contain wav data itself.  Examples of these types would be CDDA (multiple CD tracks), FSB (contains multiple sounds), DLS (contain instruments).
    The arrays of format, channel, frequency, length and blockalign should point to arrays of information based on how many subsounds are in the format.  If the number of subsounds is 0 then it should point to 1 of each attribute, the same as if the number of subsounds was 1.  If subsounds was 100 for example, each pointer should point to an array of 100 of each attribute.
    When a sound has 1 or more subsounds, you must play the individual sounds specified by first obtaining the subsound with Sound::getSubSound.
    
    [PLATFORMS]
    Win32, Win64, Linux, Linux64, Macintosh, Xbox360, PlayStation Portable, PlayStation 3, Wii, iPhone, 3GS, NGP, Android

    [SEE_ALSO]
    FMOD_SOUND_FORMAT
    FMOD_FILE_READCALLBACK      
    FMOD_FILE_SEEKCALLBACK      
    FMOD_CODEC_METADATACALLBACK
    Sound::getSubSound
    Sound::getNumSubSounds
]
*/
struct FMOD_CODEC_STATE
{
    int                         numsubsounds;  /* [in] Number of 'subsounds' in this sound.  Anything other than 0 makes it a 'container' format (ie CDDA/DLS/FSB etc which contain 1 or more su bsounds).  For most normal, single sound codec such as WAV/AIFF/MP3, this should be 0 as they are not a container for subsounds, they are the sound by itself. */
    FMOD_CODEC_WAVEFORMAT      *waveformat;    /* [in] Pointer to an array of format structures containing information about each sample.  Can be 0 or NULL if FMOD_CODEC_GETWAVEFORMAT callback is preferred.  The number of entries here must equal the number of subsounds defined in the subsound parameter. If numsubsounds = 0 then there should be 1 instance of this structure. */
    void                       *plugindata;    /* [in] Plugin writer created data the codec author wants to attach to this object. */
                                               
    void                       *filehandle;    /* [out] This will return an internal FMOD file handle to use with the callbacks provided.  */
    unsigned int                filesize;      /* [out] This will contain the size of the file in bytes. */
    FMOD_FILE_READCALLBACK      fileread;      /* [out] This will return a callable FMOD file function to use from codec. */
    FMOD_FILE_SEEKCALLBACK      fileseek;      /* [out] This will return a callable FMOD file function to use from codec.  */
    FMOD_CODEC_METADATACALLBACK metadata;      /* [out] This will return a callable FMOD metadata function to use from codec.  */
};

#endif


