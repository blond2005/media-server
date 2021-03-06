#include "rtsp-server-internal.h"

int rtsp_server_handle(struct rtsp_server_t *rtsp)
{
	int major, minor;
	const char* uri;
	const char* method;

	rtsp_get_version(rtsp->parser, &major, &minor);
	if (1 != major && 0 != minor)
	{
		//505 RTSP Version Not Supported
		return rtsp_server_reply(rtsp, 505);
	}

	if (0 != rtsp_get_header_by_name2(rtsp->parser, "CSeq", (int*)&rtsp->cseq))
	{
		// 400 Bad Request
		return rtsp_server_reply(rtsp, 400);
	}

	uri = rtsp_get_request_uri(rtsp->parser);
	method = rtsp_get_request_method(rtsp->parser);

	switch (*method)
	{
	case 'o':
	case 'O':
		if (0 == strcasecmp("OPTIONS", method))
			return rtsp_server_options(rtsp, uri);
		break;

	case 'd':
	case 'D':
		if (0 == strcasecmp("DESCRIBE", method))
			return rtsp_server_describe(rtsp, uri);
		break;

	case 's':
	case 'S':
		if (0 == strcasecmp("SETUP", method))
			return rtsp_server_setup(rtsp, uri);
		break;

	case 'p':
	case 'P':
		if (0 == strcasecmp("PLAY", method))
			return rtsp_server_play(rtsp, uri);
		else if (0 == strcasecmp("PAUSE", method))
			return rtsp_server_pause(rtsp, uri);
		break;

	case 't':
	case 'T':
		if (0 == strcasecmp("TEARDOWN", method))
			return rtsp_server_teardown(rtsp, uri);
		break;
	}

	// 501 Not implemented
	return rtsp_server_reply(rtsp, 501);
}
