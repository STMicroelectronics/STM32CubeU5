
#include "nx_api.h"

#ifdef __PRODUCT_NETXDUO__

#include "netx_mdns_test.h"

/* Frame (140 bytes) */
static const unsigned char pkt1[140] = {
0x01, 0x00, 0x5e, 0x00, 0x00, 0xfb, 0xec, 0x17, /* ..^..... */
0x2f, 0xbd, 0xa4, 0xb1, 0x08, 0x00, 0x45, 0x00, /* /.....E. */
0x00, 0x7e, 0x01, 0x4c, 0x00, 0x00, 0xff, 0x11, /* .~.L.... */
0x11, 0x75, 0xc0, 0xa8, 0x07, 0x0a, 0xe0, 0x00, /* .u...... */
0x00, 0xfb, 0x14, 0xe9, 0x14, 0xe9, 0x00, 0x6a, /* .......j */
0x06, 0xdb, 0x00, 0x00, 0x84, 0x00, 0x00, 0x00, /* ........ */
0x00, 0x02, 0x00, 0x00, 0x00, 0x01, 0x08, 0x43, /* .......C */
0x61, 0x74, 0x72, 0x6f, 0x2d, 0x50, 0x43, 0x05, /* atro-PC. */
0x6c, 0x6f, 0x63, 0x61, 0x6c, 0x00, 0x00, 0x01, /* local... */
0x80, 0x01, 0x00, 0x00, 0x00, 0x78, 0x00, 0x04, /* .....x.. */
0xc0, 0xa8, 0x07, 0x0a, 0x02, 0x31, 0x30, 0x01, /* .....10. */
0x37, 0x03, 0x31, 0x36, 0x38, 0x03, 0x31, 0x39, /* 7.168.19 */
0x32, 0x07, 0x69, 0x6e, 0x2d, 0x61, 0x64, 0x64, /* 2.in-add */
0x72, 0x04, 0x61, 0x72, 0x70, 0x61, 0x00, 0x00, /* r.arpa.. */
0x0c, 0x80, 0x01, 0x00, 0x00, 0x00, 0x78, 0x00, /* ......x. */
0x02, 0xc0, 0x0c, 0xc0, 0x0c, 0x00, 0x2f, 0x80, /* ....../. */
0x01, 0x00, 0x00, 0x00, 0x78, 0x00, 0x05, 0xc0, /* ....x... */
0x0c, 0x00, 0x01, 0x40                          /* ...@ */
};

/* Frame (79 bytes) */
static const unsigned char pkt2[79] = {
0x01, 0x00, 0x5e, 0x00, 0x00, 0xfb, 0xec, 0x17, /* ..^..... */
0x2f, 0xbd, 0xa4, 0xb1, 0x08, 0x00, 0x45, 0x00, /* /.....E. */
0x00, 0x41, 0x01, 0x4d, 0x00, 0x00, 0xff, 0x11, /* .A.M.... */
0x11, 0xb1, 0xc0, 0xa8, 0x07, 0x0a, 0xe0, 0x00, /* ........ */
0x00, 0xfb, 0x14, 0xe9, 0x14, 0xe9, 0x00, 0x2d, /* .......- */
0xbe, 0x48, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, /* .H...... */
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x5f, /* ......._ */
0x70, 0x72, 0x69, 0x6e, 0x74, 0x65, 0x72, 0x04, /* printer. */
0x5f, 0x74, 0x63, 0x70, 0x05, 0x6c, 0x6f, 0x63, /* _tcp.loc */
0x61, 0x6c, 0x00, 0x00, 0x0c, 0x00, 0x01        /* al..... */
};

/* Frame (79 bytes) */
static const unsigned char pkt3[79] = {
0x01, 0x00, 0x5e, 0x00, 0x00, 0xfb, 0xec, 0x17, /* ..^..... */
0x2f, 0xbd, 0xa4, 0xb1, 0x08, 0x00, 0x45, 0x00, /* /.....E. */
0x00, 0x41, 0x01, 0x4e, 0x00, 0x00, 0xff, 0x11, /* .A.N.... */
0x11, 0xb0, 0xc0, 0xa8, 0x07, 0x0a, 0xe0, 0x00, /* ........ */
0x00, 0xfb, 0x14, 0xe9, 0x14, 0xe9, 0x00, 0x2d, /* .......- */
0xbe, 0x48, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, /* .H...... */
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x5f, /* ......._ */
0x70, 0x72, 0x69, 0x6e, 0x74, 0x65, 0x72, 0x04, /* printer. */
0x5f, 0x74, 0x63, 0x70, 0x05, 0x6c, 0x6f, 0x63, /* _tcp.loc */
0x61, 0x6c, 0x00, 0x00, 0x0c, 0x00, 0x01        /* al..... */
};

/* Frame (75 bytes) */
static const unsigned char pkt4[75] = {
0x01, 0x00, 0x5e, 0x00, 0x00, 0xfb, 0xec, 0x17, /* ..^..... */
0x2f, 0xbd, 0xa4, 0xb1, 0x08, 0x00, 0x45, 0x00, /* /.....E. */
0x00, 0x3d, 0x01, 0x4f, 0x00, 0x00, 0xff, 0x11, /* .=.O.... */
0x11, 0xb3, 0xc0, 0xa8, 0x07, 0x0a, 0xe0, 0x00, /* ........ */
0x00, 0xfb, 0x14, 0xe9, 0x14, 0xe9, 0x00, 0x29, /* .......) */
0xa9, 0x26, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, /* .&...... */
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x5f, /* ......._ */
0x69, 0x70, 0x70, 0x04, 0x5f, 0x74, 0x63, 0x70, /* ipp._tcp */
0x05, 0x6c, 0x6f, 0x63, 0x61, 0x6c, 0x00, 0x00, /* .local.. */
0x0c, 0x00, 0x01                                /* ... */
};

/* Frame (632 bytes) */
static const unsigned char pkt5[632] = {
0x01, 0x00, 0x5e, 0x00, 0x00, 0xfb, 0x00, 0x11, /* ..^..... */
0x22, 0x33, 0x44, 0x57, 0x08, 0x00, 0x45, 0x00, /* "3DW..E. */
0x02, 0x6a, 0x00, 0x07, 0x40, 0x00, 0xff, 0x11, /* .j..@... */
0x8e, 0x3e, 0x0a, 0x00, 0x00, 0x42, 0xe0, 0x00, /* .>...B.. */
0x00, 0xfb, 0x14, 0xe9, 0x14, 0xe9, 0x02, 0x56, /* .......V */
0x66, 0x0b, 0x00, 0x00, 0x84, 0x00, 0x00, 0x00, /* f....... */
0x00, 0x02, 0x00, 0x00, 0x00, 0x08, 0x08, 0x5f, /* ......._ */
0x70, 0x72, 0x69, 0x6e, 0x74, 0x65, 0x72, 0x04, /* printer. */
0x5f, 0x74, 0x63, 0x70, 0x05, 0x6c, 0x6f, 0x63, /* _tcp.loc */
0x61, 0x6c, 0x00, 0x00, 0x0c, 0x00, 0x01, 0x00, /* al...... */
0x00, 0x00, 0x64, 0x00, 0x21, 0x0b, 0x41, 0x52, /* ..d.!.AR */
0x4d, 0x4d, 0x44, 0x4e, 0x53, 0x54, 0x65, 0x73, /* MMDNSTes */
0x74, 0x08, 0x5f, 0x70, 0x72, 0x69, 0x6e, 0x74, /* t._print */
0x65, 0x72, 0x04, 0x5f, 0x74, 0x63, 0x70, 0x05, /* er._tcp. */
0x6c, 0x6f, 0x63, 0x61, 0x6c, 0x00, 0x04, 0x5f, /* local.._ */
0x69, 0x70, 0x70, 0x04, 0x5f, 0x74, 0x63, 0x70, /* ipp._tcp */
0x05, 0x6c, 0x6f, 0x63, 0x61, 0x6c, 0x00, 0x00, /* .local.. */
0x0c, 0x00, 0x01, 0x00, 0x00, 0x00, 0x64, 0x00, /* ......d. */
0x1d, 0x0b, 0x41, 0x52, 0x4d, 0x4d, 0x44, 0x4e, /* ..ARMMDN */
0x53, 0x54, 0x65, 0x73, 0x74, 0x04, 0x5f, 0x69, /* STest._i */
0x70, 0x70, 0x04, 0x5f, 0x74, 0x63, 0x70, 0x05, /* pp._tcp. */
0x6c, 0x6f, 0x63, 0x61, 0x6c, 0x00, 0x0b, 0x41, /* local..A */
0x52, 0x4d, 0x4d, 0x44, 0x4e, 0x53, 0x54, 0x65, /* RMMDNSTe */
0x73, 0x74, 0x05, 0x6c, 0x6f, 0x63, 0x61, 0x6c, /* st.local */
0x00, 0x00, 0x01, 0x80, 0x01, 0x00, 0x00, 0x00, /* ........ */
0x78, 0x00, 0x04, 0x0a, 0x00, 0x00, 0x42, 0x0b, /* x.....B. */
0x41, 0x52, 0x4d, 0x4d, 0x44, 0x4e, 0x53, 0x54, /* ARMMDNST */
0x65, 0x73, 0x74, 0x05, 0x6c, 0x6f, 0x63, 0x61, /* est.loca */
0x6c, 0x00, 0x00, 0x2f, 0x80, 0x01, 0x00, 0x00, /* l../.... */
0x00, 0x78, 0x00, 0x16, 0x0b, 0x41, 0x52, 0x4d, /* .x...ARM */
0x4d, 0x44, 0x4e, 0x53, 0x54, 0x65, 0x73, 0x74, /* MDNSTest */
0x05, 0x6c, 0x6f, 0x63, 0x61, 0x6c, 0x00, 0x00, /* .local.. */
0x01, 0x40, 0x0b, 0x41, 0x52, 0x4d, 0x4d, 0x44, /* .@.ARMMD */
0x4e, 0x53, 0x54, 0x65, 0x73, 0x74, 0x08, 0x5f, /* NSTest._ */
0x70, 0x72, 0x69, 0x6e, 0x74, 0x65, 0x72, 0x04, /* printer. */
0x5f, 0x74, 0x63, 0x70, 0x05, 0x6c, 0x6f, 0x63, /* _tcp.loc */
0x61, 0x6c, 0x00, 0x00, 0x21, 0x80, 0x01, 0x00, /* al..!... */
0x00, 0x00, 0x64, 0x00, 0x19, 0x00, 0x00, 0x00, /* ..d..... */
0x00, 0x00, 0x50, 0x0b, 0x41, 0x52, 0x4d, 0x4d, /* ..P.ARMM */
0x44, 0x4e, 0x53, 0x54, 0x65, 0x73, 0x74, 0x05, /* DNSTest. */
0x6c, 0x6f, 0x63, 0x61, 0x6c, 0x00, 0x0b, 0x41, /* local..A */
0x52, 0x4d, 0x4d, 0x44, 0x4e, 0x53, 0x54, 0x65, /* RMMDNSTe */
0x73, 0x74, 0x08, 0x5f, 0x70, 0x72, 0x69, 0x6e, /* st._prin */
0x74, 0x65, 0x72, 0x04, 0x5f, 0x74, 0x63, 0x70, /* ter._tcp */
0x05, 0x6c, 0x6f, 0x63, 0x61, 0x6c, 0x00, 0x00, /* .local.. */
0x10, 0x80, 0x01, 0x00, 0x00, 0x00, 0x64, 0x00, /* ......d. */
0x01, 0x00, 0x0b, 0x41, 0x52, 0x4d, 0x4d, 0x44, /* ...ARMMD */
0x4e, 0x53, 0x54, 0x65, 0x73, 0x74, 0x08, 0x5f, /* NSTest._ */
0x70, 0x72, 0x69, 0x6e, 0x74, 0x65, 0x72, 0x04, /* printer. */
0x5f, 0x74, 0x63, 0x70, 0x05, 0x6c, 0x6f, 0x63, /* _tcp.loc */
0x61, 0x6c, 0x00, 0x00, 0x2f, 0x80, 0x01, 0x00, /* al../... */
0x00, 0x00, 0x78, 0x00, 0x28, 0x0b, 0x41, 0x52, /* ..x.(.AR */
0x4d, 0x4d, 0x44, 0x4e, 0x53, 0x54, 0x65, 0x73, /* MMDNSTes */
0x74, 0x08, 0x5f, 0x70, 0x72, 0x69, 0x6e, 0x74, /* t._print */
0x65, 0x72, 0x04, 0x5f, 0x74, 0x63, 0x70, 0x05, /* er._tcp. */
0x6c, 0x6f, 0x63, 0x61, 0x6c, 0x00, 0x00, 0x05, /* local... */
0x00, 0x00, 0x80, 0x00, 0x40, 0x0b, 0x41, 0x52, /* ....@.AR */
0x4d, 0x4d, 0x44, 0x4e, 0x53, 0x54, 0x65, 0x73, /* MMDNSTes */
0x74, 0x04, 0x5f, 0x69, 0x70, 0x70, 0x04, 0x5f, /* t._ipp._ */
0x74, 0x63, 0x70, 0x05, 0x6c, 0x6f, 0x63, 0x61, /* tcp.loca */
0x6c, 0x00, 0x00, 0x21, 0x80, 0x01, 0x00, 0x00, /* l..!.... */
0x00, 0x64, 0x00, 0x19, 0x00, 0x00, 0x00, 0x00, /* .d...... */
0x00, 0x50, 0x0b, 0x41, 0x52, 0x4d, 0x4d, 0x44, /* .P.ARMMD */
0x4e, 0x53, 0x54, 0x65, 0x73, 0x74, 0x05, 0x6c, /* NSTest.l */
0x6f, 0x63, 0x61, 0x6c, 0x00, 0x0b, 0x41, 0x52, /* ocal..AR */
0x4d, 0x4d, 0x44, 0x4e, 0x53, 0x54, 0x65, 0x73, /* MMDNSTes */
0x74, 0x04, 0x5f, 0x69, 0x70, 0x70, 0x04, 0x5f, /* t._ipp._ */
0x74, 0x63, 0x70, 0x05, 0x6c, 0x6f, 0x63, 0x61, /* tcp.loca */
0x6c, 0x00, 0x00, 0x10, 0x80, 0x01, 0x00, 0x00, /* l....... */
0x00, 0x64, 0x00, 0x01, 0x00, 0x0b, 0x41, 0x52, /* .d....AR */
0x4d, 0x4d, 0x44, 0x4e, 0x53, 0x54, 0x65, 0x73, /* MMDNSTes */
0x74, 0x04, 0x5f, 0x69, 0x70, 0x70, 0x04, 0x5f, /* t._ipp._ */
0x74, 0x63, 0x70, 0x05, 0x6c, 0x6f, 0x63, 0x61, /* tcp.loca */
0x6c, 0x00, 0x00, 0x2f, 0x80, 0x01, 0x00, 0x00, /* l../.... */
0x00, 0x78, 0x00, 0x24, 0x0b, 0x41, 0x52, 0x4d, /* .x.$.ARM */
0x4d, 0x44, 0x4e, 0x53, 0x54, 0x65, 0x73, 0x74, /* MDNSTest */
0x04, 0x5f, 0x69, 0x70, 0x70, 0x04, 0x5f, 0x74, /* ._ipp._t */
0x63, 0x70, 0x05, 0x6c, 0x6f, 0x63, 0x61, 0x6c, /* cp.local */
0x00, 0x00, 0x05, 0x00, 0x00, 0x80, 0x00, 0x40  /* .......@ */
};

/* Frame (101 bytes) */
static const unsigned char pkt6[101] = {
0x01, 0x00, 0x5e, 0x00, 0x00, 0xfb, 0xec, 0x17, /* ..^..... */
0x2f, 0xbd, 0xa4, 0xb1, 0x08, 0x00, 0x45, 0x00, /* /.....E. */
0x00, 0x57, 0x01, 0x50, 0x00, 0x00, 0xff, 0x11, /* .W.P.... */
0x11, 0x98, 0xc0, 0xa8, 0x07, 0x0a, 0xe0, 0x00, /* ........ */
0x00, 0xfb, 0x14, 0xe9, 0x14, 0xe9, 0x00, 0x43, /* .......C */
0x10, 0xb0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, /* ........ */
0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x04, 0x5f, /* ......._ */
0x69, 0x70, 0x70, 0x04, 0x5f, 0x74, 0x63, 0x70, /* ipp._tcp */
0x05, 0x6c, 0x6f, 0x63, 0x61, 0x6c, 0x00, 0x00, /* .local.. */
0x0c, 0x00, 0x01, 0xc0, 0x0c, 0x00, 0x0c, 0x00, /* ........ */
0x01, 0x00, 0x00, 0x00, 0x64, 0x00, 0x0e, 0x0b, /* ....d... */
0x41, 0x52, 0x4d, 0x4d, 0x44, 0x4e, 0x53, 0x54, /* ARMMDNST */
0x65, 0x73, 0x74, 0xc0, 0x0c                    /* est.. */
};

/* Frame (105 bytes) */
static const unsigned char pkt7[105] = {
0x01, 0x00, 0x5e, 0x00, 0x00, 0xfb, 0xec, 0x17, /* ..^..... */
0x2f, 0xbd, 0xa4, 0xb1, 0x08, 0x00, 0x45, 0x00, /* /.....E. */
0x00, 0x5b, 0x01, 0x51, 0x00, 0x00, 0xff, 0x11, /* .[.Q.... */
0x11, 0x93, 0xc0, 0xa8, 0x07, 0x0a, 0xe0, 0x00, /* ........ */
0x00, 0xfb, 0x14, 0xe9, 0x14, 0xe9, 0x00, 0x47, /* .......G */
0x26, 0xd2, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, /* &....... */
0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x08, 0x5f, /* ......._ */
0x70, 0x72, 0x69, 0x6e, 0x74, 0x65, 0x72, 0x04, /* printer. */
0x5f, 0x74, 0x63, 0x70, 0x05, 0x6c, 0x6f, 0x63, /* _tcp.loc */
0x61, 0x6c, 0x00, 0x00, 0x0c, 0x00, 0x01, 0xc0, /* al...... */
0x0c, 0x00, 0x0c, 0x00, 0x01, 0x00, 0x00, 0x00, /* ........ */
0x63, 0x00, 0x0e, 0x0b, 0x41, 0x52, 0x4d, 0x4d, /* c...ARMM */
0x44, 0x4e, 0x53, 0x54, 0x65, 0x73, 0x74, 0xc0, /* DNSTest. */
0x0c                                            /* . */
};

static MDNS_SERVICE mdns_service_0 = {"ARMMDNSTest", "_printer._tcp", NX_NULL, NX_NULL, 100, 0, 0, 80, NX_MDNS_RR_SET_UNIQUE, 0};
static MDNS_SERVICE mdns_service_1 = {"ARMMDNSTest", "_ipp._tcp", NX_NULL, NX_NULL, 100, 0, 0, 80, NX_MDNS_RR_SET_UNIQUE, 0};


MDNS_TEST_SEQ mdns_response_aggregation[] = {
    {TITLE, "Response aggregation", 20, 0},
    {MDNS_SERVICE_ADD, (char*)&mdns_service_0, 0, 0},
    {MDNS_SERVICE_ADD, (char*)&mdns_service_1, 0, 0},
    {WAIT, NX_NULL, 0, 5},
    {DUMP, NX_NULL, 0, 0},

    /* Inject two queries. */
    {INJECT, (char*)&pkt3[0], sizeof(pkt3), 0},

    /* Sleep 10ms. */
    {MDNS_WAIT_TICK, NX_NULL, 0, 1},

    {INJECT, (char*)&pkt4[0], sizeof(pkt4), 0},

    /* Check response. */
    {MDNS_CHECK_DATA_V4, (char*)&pkt5[0], sizeof(pkt5), 2},
};

int mdns_response_aggregation_size = sizeof(mdns_response_aggregation) / sizeof(MDNS_TEST_SEQ);

#endif /* __PRODUCT_NETXDUO__  */
