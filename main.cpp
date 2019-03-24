#include <pjsua.h>
#include <iostream>
#include "settings.h"

#define THIS_FILE "main.cpp"

int StartSip(void);
int EndSip(void);



int main (void)
{
	StartSip();
	
	/*
	 * 
	 *  in here we do all the radio interface or whatever else we want
	 * 
	 * 
	 */
	getchar();
	EndSip();
	return 0;
}
















/* Display error and exit application */
static void error_exit(const char *title, pj_status_t status)
{
  pjsua_perror(THIS_FILE, title, status);
  pjsua_destroy();
  exit(1);
}

 /* Callback called by the library when call's state has changed */
static void on_call_state(pjsua_call_id call_id, pjsip_event *e)
{
 printf("\n\n====================== on_call_state callback started =====================\n\n");
 pjsua_call_info ci;

 PJ_UNUSED_ARG(e);

 pjsua_call_get_info(call_id, &ci);
 PJ_LOG(3,(THIS_FILE, "Call %d state=%.*s", call_id,
 (int)ci.state_text.slen,
 ci.state_text.ptr));
 printf("\n\n====================== on_call_state callback Done =====================\n\n");
}


/* Callback called by the library when call's media state has changed */
static void on_call_media_state(pjsua_call_id call_id)
{
printf("\n\n====================== on_call_media_state callback started =====================\n\n");
    pjsua_call_info ci;

    pjsua_call_get_info(call_id, &ci);
 
    if (ci.media_status == PJSUA_CALL_MEDIA_ACTIVE) {
        // When media is active, connect call to sound device.
        pjsua_conf_connect(ci.conf_slot, 0);
        pjsua_conf_connect(0, ci.conf_slot);
    }
printf("\n\n====================== on_call_media_state callback Done =====================\n\n");
}





int StartSip(void)
{
 pjsua_acc_id acc_id;
 pj_status_t status;

  /* Create pjsua first! */
  status = pjsua_create();
  if (status != PJ_SUCCESS) error_exit("Error in pjsua_create()", status);
 


  /* Init pjsua */
{
  pjsua_config cfg;
  pjsua_logging_config log_cfg;
               
  pjsua_config_default(&cfg);
  cfg.cb.on_call_state = &on_call_state;
  cfg.cb.on_call_media_state = &on_call_media_state; 
  
  pjsua_logging_config_default(&log_cfg);
  log_cfg.console_level = 4;
 
  status = pjsua_init(&cfg, &log_cfg, NULL);
  if (status != PJ_SUCCESS) error_exit("Error in pjsua_init()", status);
  printf("\n\n====================== PJSUA Done =====================\n\n");
  }



  /* Add UDP transport. */
  {
   pjsua_transport_config cfg;
 
   pjsua_transport_config_default(&cfg);
   cfg.port = 5060;
   status = pjsua_transport_create(PJSIP_TRANSPORT_UDP, &cfg, NULL);
   if (status != PJ_SUCCESS) error_exit("Error creating transport", status);
  printf("\n\n====================== UDP Done =====================\n\n");
  }

/* Initialization is done, now start pjsua */
  status = pjsua_start();
  if (status != PJ_SUCCESS) error_exit("Error starting pjsua", status);
/* Register to SIP server by creating SIP account. */

 {
   pjsua_acc_config cfg;
   pjsua_acc_config_default(&cfg);
   cfg.id = pj_str((char*)"sip:" SIP_USER "@" SIP_DOMAIN);
   cfg.reg_uri = pj_str((char*)"sip:" SIP_DOMAIN);
   cfg.cred_count = 1;
   cfg.cred_info[0].realm = pj_str((char*)"asterisk");
   cfg.cred_info[0].scheme = pj_str((char*)"digest");
   cfg.cred_info[0].username = pj_str((char*)SIP_USER);
   cfg.cred_info[0].data_type = PJSIP_CRED_DATA_PLAIN_PASSWD;
   cfg.cred_info[0].data = pj_str((char*)SIP_PASSWD);
 
  printf("\n\n====================== Registering Account =====================\n\n");
   status = pjsua_acc_add(&cfg, PJ_TRUE, &acc_id);
   if (status != PJ_SUCCESS) 
   {
	   error_exit((char*)"\n\n====================== Error adding account =====================\n\n", status);
   }
  printf("\n\n====================== Account registered =====================\n\n");
  }


  /*  make call to the URL. */
  printf("\n\n======================Making the call=====================\n\n");
  pj_str_t uri = pj_str((char*) SIP_HOME_EXT SIP_DOMAIN);
  status = pjsua_call_make_call(acc_id, &uri, 0, NULL, NULL, NULL);
  if (status != PJ_SUCCESS) 
  {
	  error_exit("\n\n===================== Error making call =====================\n\n", status);
  }
  printf("\n\n====================== Made the call =====================\n\n");  


  return 0;
} 
 
 int EndSip(void)
{
 pjsua_call_hangup_all();
 /* Destroy pjsua */
 pjsua_destroy();
 return 0;
}
