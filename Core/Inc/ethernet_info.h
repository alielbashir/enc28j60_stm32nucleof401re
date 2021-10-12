
typedef enum
{

	NETINFO_STATIC = 1,    ///< Static IP configuration by manually.
	NETINFO_DHCP           ///< Dynamic IP configruation from a DHCP sever

}dhcp_mode;


typedef struct wiz_NetInfo_t
{

	uint8_t mac[6];  ///< Source Mac Address
	uint8_t ip[4];   ///< Source IP Address
	uint8_t sn[4];   ///< Subnet Mask
	uint8_t gw[4];   ///< Gateway IP Address
	uint8_t dns[4];  ///< DNS server IP Address
	dhcp_mode dhcp;  ///< 1 - Static, 2 - DHCP

}wiz_NetInfo;
