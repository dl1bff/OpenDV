/*
 *   Copyright (C) 2009-2013 by Jonathan Naylor G4KLX
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 2 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program; if not, write to the Free Software
 *   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#ifndef	SoundCardRepeaterTXThread_H
#define	SoundCardRepeaterTXThread_H

#include "SoundCardRepeaterThread.h"
#include "DStarGMSKModulator.h"
#include "DVTOOLFileWriter.h"
#include "SlowDataEncoder.h"
#include "WAVFileWriter.h"
#include "HeaderData.h"
#include "AudioDelay.h"
#include "RingBuffer.h"
#include "PTTDelay.h"
#include "AMBEFEC.h"
#include "Timer.h"

#include <wx/wx.h>

class CSoundCardRepeaterTXThread : public ISoundCardRepeaterThread {
public:
	CSoundCardRepeaterTXThread();
	virtual ~CSoundCardRepeaterTXThread();

	virtual void callback(const wxFloat32* input, wxFloat32* output, unsigned int nSamples, int id);

	virtual void setCallsign(const wxString& callsign, const wxString& gateway, DSTAR_MODE mode, ACK_TYPE ack, bool restriction, bool rpt1Validation, bool dtmfBlanking);
	virtual void setProtocolHandler(CRepeaterProtocolHandler* handler);
	virtual void setSoundCard(CSoundCardReaderWriter* soundcard, wxFloat32 rxLevel, wxFloat32 txLevel, SQUELCH_MODE squelchMode, wxFloat32 squelchLevel, bool rxInvert, bool txInvert);
	virtual void setController(CExternalController* controller, int pttDelay);
	virtual void setTimes(unsigned int timeout, unsigned int ackTime, unsigned int hangTime);
	virtual void setBeacon(unsigned int time, const wxString& text, bool voice, TEXT_LANG language);
	virtual void setAnnouncement(bool enabled, unsigned int time, const wxString& recordRPT1, const wxString& recordRPT2, const wxString& deleteRPT1, const wxString& deleteRPT2);
	virtual void setControl(bool enabled, const wxString& rpt1Callsign, const wxString& rpt2Callsign, const wxString& shutdown, const wxString& startup, const wxString& status1, const wxString& status2, const wxString& status3, const wxString& status4, const wxString& status5, const wxString& command1, const wxString& command1Line, const wxString& command2, const wxString& command2Line, const wxString& command3, const wxString& command3Line, const wxString& command4, const wxString& command4Line, const wxString& output1, const wxString& output2, const wxString& output3, const wxString& output4);
	virtual void setActiveHang(unsigned int time);
	virtual void setOutputs(bool out1, bool out2, bool out3, bool out4);
	virtual void setLogging(bool logging, const wxString& dir);
	virtual void setReader(CWAVFileReader* reader);
	virtual void setBlackList(CCallsignList* list);
	virtual void setGreyList(CCallsignList* list);

	virtual void setRXLevel(wxFloat32 level);
	virtual void setTXLevel(wxFloat32 level);
	virtual void setSquelchLevel(wxFloat32 level);

	virtual void shutdown();
	virtual void startup();

	virtual void command1();
	virtual void command2();
	virtual void command3();
	virtual void command4();

	virtual CSoundCardRepeaterStatusData* getStatus();

	virtual void run();
	virtual void kill();

private:
	CSoundCardReaderWriter*   m_soundcard;
	CRepeaterProtocolHandler* m_protocolHandler;
	CExternalController*      m_controller;
	CPTTDelay*                m_pttDelay;
	CAudioDelay*              m_audioDelay;
	CRingBuffer<wxFloat32>    m_outBuffer;
	CRingBuffer<bool>         m_networkBuffer;
	unsigned int              m_networkRun;
	bool                      m_networkStarted;
	wxString                  m_rptCallsign;
	CDStarGMSKModulator       m_modulator;
	CWAVFileWriter*           m_writer;
	CHeaderData*              m_header;
	unsigned char             m_networkSeqNo;
	CTimer                    m_watchdogTimer;
	CTimer                    m_pollTimer;
	CTimer                    m_hangTimer;
	DSTAR_RPT_STATE           m_rptState;
	bool                      m_tx;
	bool                      m_killed;
	CTimer                    m_activeHangTimer;
	unsigned char*            m_lastData;
	CAMBEFEC                  m_ambe;
	wxStopWatch               m_headerTime;
	wxStopWatch               m_packetTime;
	unsigned int              m_packetCount;
	unsigned int              m_packetSilence;

	void receiveNetwork();
	void transmitNetworkHeader(CHeaderData& header);
	void transmit();
	void transmitNetwork();
	void transmitBits(const bool* bits, unsigned int length);

	void processNetworkHeader(CHeaderData* header);
	unsigned int processNetworkFrame(unsigned char* data, unsigned int length, unsigned char seqNo);
	void endOfNetworkData();
	void clock(unsigned int ms);
};

#endif
