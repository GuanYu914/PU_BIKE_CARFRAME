#include <MySQL_Connection.h>
#include <MySQL_Cursor.h>
#include <ESP8266WiFi.h>
#include <Servo.h>

/*------------WIFISSID--------------*/
#define WIFI_SSID           "PU_BIKE"
#define WIFI_PASS           "makerbar"
/*------------TIMELEVEL-------------*/
/*Unit: ms*/
#define WIFI_BLINK_TIME_LEVEL      100
/*Unit: ms*/
#define READ_SW_TIME_LEVEL          10
/*Unit: ms*/
#define SEND_MySQL_QUERY_TIME_LEVEL 10
/*Unit: ms*/
#define DETECT_STATUS_TIME_LEVEL    10                
/*-------------RGBLED---------------*/
/*GPIO14 --> D13*/
#define R_LED_PIN                   14
/*GPIO13 --> D11*/
#define G_LED_PIN                   13
/*GPIO12 --> D12*/
#define B_LED_PIN                   12
/*---------------SW-----------------*/
/*GPIO16 --> D2*/
#define SW_Pin                     16
/*----------MYSQL&SERIAL--------------------------------*/
#define SERIAL_BUAD_RATE                            115200
#define MySQL_SERVER_PORT                             7777
#define DB_NAME                                  "PU_Bike"
#define TABLE_NAME                                  "bike"
#define CARFRAME_ID                              "\"A01\""
#define FETCH_ID_COL                             "BIKE_ID"
#define FETCH_RESERVER_COL                      "reserver"
#define FETCH_RESERVER_RENT_FLAG_COL  "RESERVER_RENT_FLAG"
#define UPLOAD_ID_COL                            "BIKE_ID"
#define UPLOAD_SW_ID_COL                       "SW_STATUS"
#define UPLOAD_RESERVER_RENT_FLAG_COL "RESERVER_RENT_FLAG"
#define SEARCH_COL_NAME  "PARKING_ID"
/*-------------SERVO------------------------------------*/
/*GPIO04 --> D4*/
#define SERVER_MOTOR_PIN           4
#define SERVO_MAX_PULSE_WIDTH   1700
#define SERVO_MIN_PULSE_WIDTH    550
#define SERVO_DELAY               10

char * non_reserver_temp_status = "";
char * reserver_temp_status     = "";
char * current_id               = "";
char * MySQL_db_user            = "Bike_manager";
char * MySQL_db_pass            = "manager";
int reserver_rent_flag          = 0;

WiFiClient WIFI_client;
Servo car_frame_servo_motor;
MySQL_Connection MySQL_connector((Client *)&WIFI_client);
/*100.100.100.102 is MySQL host ip*/
IPAddress MySQL_server_host_ipAdderss(100, 100, 100, 102);

void setup()
{
        Serial.begin(SERIAL_BUAD_RATE);
        pinMode(SW_Pin,    INPUT);
        pinMode(R_LED_PIN, OUTPUT);
        pinMode(G_LED_PIN, OUTPUT);
        pinMode(B_LED_PIN, OUTPUT);
        pinMode(SERVER_MOTOR_PIN, OUTPUT);
        car_frame_servo_motor.attach(SERVER_MOTOR_PIN);
        RGBLED_turnoff();
        SERVO_control("INIT");
        WIFI_connect();
        Serial.println("Initialize completed.");
}

void loop()
{
        if(WiFi.status() == WL_CONNECTED)
        {
                if(MySQL_connector.connected())
                {
                        if(!strlen(MySQL_fetch_col_data(FETCH_RESERVER_COL)))
                        {
                                /*ENTER NON-RESERVER STATUS*/
                                /*reserver rent bike when enter this case*/
                                if(!strcmp(MySQL_fetch_col_data(FETCH_RESERVER_RENT_FLAG_COL), "1"))
                                {
                                        MySQL_upload_local_data("0", UPLOAD_RESERVER_RENT_FLAG_COL);
                                        MySQL_upload_local_data("\"\"", UPLOAD_ID_COL);
                                        goto RENT;         
                                }
                                if(SW_readsignal())
                                {
                                        MySQL_upload_local_data("1", UPLOAD_SW_ID_COL);
                                        current_id = MySQL_fetch_col_data(FETCH_ID_COL);
                                        if(strlen(current_id))
                                        {
                                                /*STATUS：RETURN STATUS IN NON-RESERVER STATUS*/
                                                /*enter return status when return from reserver status */
                                                if(!strcmp(non_reserver_temp_status, "RETURN"))
                                                {
                                                        RGBLED_control("RETURN");
                                                        Serial.println("RETURN STATUS UNDER SAME STATUS.");
                                                }
                                                if(strcmp(non_reserver_temp_status, "RETURN"))
                                                {
                                                        non_reserver_temp_status = "RETURN";
                                                        RGBLED_control("RETURN");
                                                        SERVO_control("RETURN");
                                                        Serial.println("RETURN STATUS.");
                                                }
                                        }
                                        else
                                        {
                                                /*STATUS：RENT STATUS IN NON-RESERVER STATUS*/
                                                /*avoid to convert WAITTING STATUS -> RENT STATUS*/
                                                if(strcmp(non_reserver_temp_status, "RENT") && strcmp(non_reserver_temp_status, "WAITTING"))
                                                {
                                                        RENT:
                                                                non_reserver_temp_status = "RENT";
                                                                RGBLED_control("RENT");
                                                                SERVO_control("RENT");
                                                                Serial.println("RENT STATUS.");
                                                }
                                        } 
                                }else
                                {
                                        MySQL_upload_local_data("0", UPLOAD_SW_ID_COL);
                                        current_id = MySQL_fetch_col_data(FETCH_ID_COL);
                                        if(strlen(current_id))
                                        {
                                                /*STATUS：ERROR STATUS IN NON-RESERVER STATUS*/
                                                if(strcmp(non_reserver_temp_status, "ERROR"))
                                                {
                                                        non_reserver_temp_status = "ERROR";
                                                        RGBLED_control("ERROR");
                                                        Serial.println("ERROR STATUS.");
                                                }
                                        }
                                        else
                                        {
                                                /*STATUS：WAITTING STATUS IN NON-RESERVER STATUS*/
                                                if(strcmp(non_reserver_temp_status, "WAITTING"))
                                                {
                                                        non_reserver_temp_status = "WAITTING";
                                                        RGBLED_control("WAITTING");
                                                        SERVO_control("WAITTING");
                                                        Serial.println("WAITTING STATUS.");
                                                }
                                        }
                                }
                        }else
                        {
                                /*ENTER RESERVER STATUS*/
                                /*ATTENTION:
                                1. only return status can change to return status under non-reserver state 
                                */
                                if(SW_readsignal())
                                {
                                        MySQL_upload_local_data("1", UPLOAD_SW_ID_COL);
                                        current_id = MySQL_fetch_col_data(FETCH_ID_COL);
                                        if(strlen(current_id))
                                        {
                                                /*STATUS：RETURN STATUS IN RESERVER STATUS*/
                                                /*enter return status when return from non-reserver status */
                                                if(!strcmp(reserver_temp_status, "RETURN"))
                                                {
                                                        RGBLED_control("RESERVER");
                                                        Serial.println("RETURN STATUS UNDER SAME STATUS HAVING RESERVER.");
                                                }
                                                if(strcmp(reserver_temp_status, "RETURN"))
                                                {
                                                        reserver_temp_status = "RETURN";
                                                        RGBLED_control("RESERVER");
                                                        Serial.println("RETURN STATUS UNDER HAVING RESERVER.");
                                                }
                                        }
                                        else
                                        {
                                                /*STATUS：RENT STATUS IN RESERVER STATUS*/
                                                if(strcmp(reserver_temp_status, "RENT"))
                                                {
                                                        reserver_temp_status = "RENT";
                                                        RGBLED_control("ERROR");
                                                        Serial.println("ERROR：RENT STATUS UNDER HAVING RESERVER.");
                                                }
                                        } 
                                }else
                                {
                                        MySQL_upload_local_data("0", UPLOAD_SW_ID_COL);
                                        current_id = MySQL_fetch_col_data(FETCH_ID_COL);
                                        if(strlen(current_id))
                                        {
                                                /*STATUS：ERROR STATUS IN RESERVER STATUS*/
                                                if(strcmp(reserver_temp_status, "ERROR"))
                                                {
                                                        reserver_temp_status = "ERROR";
                                                        RGBLED_control("ERROR");
                                                        Serial.println("ERROR STATUS UNDER HAVING RESERVER.");
                                                }
                                        }
                                        else
                                        {
                                                /*STATUS：WAITTING IN RESERVER STATUS*/
                                                if(strcmp(reserver_temp_status, "WAITTING"))
                                                {
                                                        reserver_temp_status = "WAITTING";
                                                        RGBLED_control("ERROR");
                                                        Serial.println("ERROR：WAITTING STATUS UNDER HAVING RESERVER.");
                                                }
                                                        
                                        }
                                }
                        }
                }
                else
                {
                        MySQL_connector.close();
                        MySQL_connect();
                }
        }
        else
        {
                WIFI_connect();
        }
}

void WIFI_blink_led(void)
{
        RGBLED_turnoff();
        digitalWrite(G_LED_PIN, HIGH);
        delay(WIFI_BLINK_TIME_LEVEL);
        RGBLED_turnoff();
        digitalWrite(G_LED_PIN, LOW);
        delay(WIFI_BLINK_TIME_LEVEL);
}

int SW_readsignal(void)
{
        delay(READ_SW_TIME_LEVEL);
        int sw_value = digitalRead(SW_Pin);
        Serial.print("sw value: ");
        Serial.println(sw_value);
        return sw_value;
}

void WIFI_connect(void)
{
        WiFi.begin(WIFI_SSID, WIFI_PASS);
        RGBLED_control("WIFI_NOT_CONNECTED");
        while (WiFi.status() != WL_CONNECTED)
        {
                /*Blink LED when ESP8266 chip doesn't connect the specific wifi ssid*/
                delay(100);
                //WIFI_blink_led();
        }
        WIFI_print_status();
}

void WIFI_print_status(void)
{
        Serial.println();
        Serial.print("SSID: ");
        Serial.println(WiFi.SSID());
        Serial.print("IP Adderss: ");
        Serial.println(WiFi.localIP());
        long rssi = WiFi.RSSI();
        Serial.print("signal strength (RSSI):");
        Serial.print(rssi);
        Serial.println(" dBm");
}

boolean MySQL_connect(void)
{
        if(MySQL_connector.connect(MySQL_server_host_ipAdderss, MySQL_SERVER_PORT, MySQL_db_user, MySQL_db_pass))
        {
                Serial.println("Estabilish connection successfully.");
                return true;
        }
        else
        {
                RGBLED_control("SQL_NOT_CONNECTED");
                Serial.println("Estabilish connection unsuccessfully.");
                return false;
        }

}

void MySQL_upload_local_data(char* data_value, const char *COL_DATA_NAME)
{
        MySQL_Cursor *MySQL_Cursor_Mem = new MySQL_Cursor(&MySQL_connector);
        if(MySQL_connector.connected())
        {
                char upload_query_string[50] = "";
                char select_database_query_string[15] = "";
                char temp_data_value[12];
                /*Select MySQL DATABASE NMAE which define by DB_NAME macro name*/
                strncpy(select_database_query_string, "USE ", strlen("USE "));
                strncat(select_database_query_string, DB_NAME, strlen(DB_NAME));
                MySQL_Cursor_Mem -> execute(select_database_query_string);
                /*upload sw_value to specific column name*/
                strncpy(upload_query_string, "UPDATE ", strlen("UPDATE "));
                strncat(upload_query_string, TABLE_NAME, strlen(TABLE_NAME));
                strncat(upload_query_string, " SET ", strlen(" SET "));
                strncat(upload_query_string, COL_DATA_NAME, strlen(COL_DATA_NAME));
                strncat(upload_query_string, " = ", strlen(" = "));
                strncat(upload_query_string, data_value, strlen(data_value));
                strncat(upload_query_string, " WHERE ", strlen(" WHERE "));
                strncat(upload_query_string, SEARCH_COL_NAME, strlen(SEARCH_COL_NAME));
                strncat(upload_query_string, " = ", strlen(" = "));
                strncat(upload_query_string, CARFRAME_ID, strlen(CARFRAME_ID));
                MySQL_Cursor_Mem -> execute(upload_query_string);
        }else
        {
                MySQL_connector.close();
                MySQL_connect();
        }
        delete MySQL_Cursor_Mem;
}

char * MySQL_fetch_col_data(const char * COL_DATA_NAME)
{
        MySQL_Cursor *MySQL_Cursor_Mem = new MySQL_Cursor(&MySQL_connector);
        char * row_data = "";
        if(MySQL_connector.connected())
        {
                row_values * row = NULL;
                char insert_query_string[50] = "";
                char select_database_query_string[15] = "";
                /*select MySQL DATABASE NMAE which define by DB_NAME macro name*/
                strncpy(select_database_query_string, "USE ", strlen("USE "));
                strncat(select_database_query_string, DB_NAME, strlen(DB_NAME));
                MySQL_Cursor_Mem -> execute(select_database_query_string);
                /*use given column name to fetch specific column value */
                strncpy(insert_query_string, "SELECT ", strlen("SELECT "));
                strncat(insert_query_string, COL_DATA_NAME, strlen(COL_DATA_NAME));
                strncat(insert_query_string, " FROM ", strlen(" FROM "));
                strncat(insert_query_string, TABLE_NAME, strlen(TABLE_NAME));
                strncat(insert_query_string, " WHERE ", strlen(" WHERE "));
                strncat(insert_query_string, SEARCH_COL_NAME, strlen(SEARCH_COL_NAME));
                strncat(insert_query_string, " = ", strlen(" = "));
                strncat(insert_query_string, CARFRAME_ID, strlen(CARFRAME_ID));
                MySQL_Cursor_Mem -> execute(insert_query_string);
                MySQL_Cursor_Mem -> get_columns();
                do
                {
                        row = MySQL_Cursor_Mem -> get_next_row();
                        if (row != NULL)
                        {
                                strcpy(row_data, row -> values[0]);
                        }


                }while (row != NULL);
        }else
        {
                MySQL_connector.close();
                MySQL_connect();
        }
        delete MySQL_Cursor_Mem;
        return row_data;
}

void RGBLED_turnoff(void)
{
        digitalWrite(R_LED_PIN, LOW);
        digitalWrite(G_LED_PIN, LOW);
        digitalWrite(B_LED_PIN, LOW);
}

void RGBLED_control(char * status)
{
        if (!strcmp(status, "ERROR"))
        {
                RGBLED_turnoff();
                digitalWrite(R_LED_PIN, HIGH);
                digitalWrite(B_LED_PIN, HIGH);
                return;
        }
        if(!strcmp(status, "RESERVER"))
        {
                RGBLED_turnoff();
                digitalWrite(B_LED_PIN, HIGH);
        }
        if (!strcmp(status, "RETURN"))
        {
                RGBLED_turnoff();
                digitalWrite(R_LED_PIN, HIGH);
                digitalWrite(G_LED_PIN, HIGH);
                digitalWrite(B_LED_PIN, HIGH);
        }
        if (!strcmp(status, "RENT"))
        {
                RGBLED_turnoff();
                digitalWrite(G_LED_PIN, HIGH);
        }
        if (!strcmp(status, "WAITTING"))
        {
                RGBLED_turnoff();
                digitalWrite(R_LED_PIN, HIGH);
        }
        if (!strcmp(status, "SQL_NOT_CONNECTED"))
        {
                RGBLED_turnoff();
                digitalWrite(R_LED_PIN, HIGH);
                digitalWrite(B_LED_PIN, HIGH);
        }
        if (!strcmp(status, "WIFI_NOT_CONNECTED"))
        {
                RGBLED_turnoff();
                digitalWrite(R_LED_PIN, HIGH);
                digitalWrite(B_LED_PIN, HIGH);
        }
}

void SERVO_control(char * bike_status)
{
        if (!strcmp(bike_status, "ERROR"))
        {
                return;
        }
        if (!strcmp(bike_status, "INIT"))
        {
                delay(SERVO_DELAY);
                car_frame_servo_motor.writeMicroseconds(SERVO_MIN_PULSE_WIDTH);
        }
        if (!strcmp(bike_status, "RETURN"))
        {
                for (int i = SERVO_MIN_PULSE_WIDTH; i < SERVO_MAX_PULSE_WIDTH; i+=10)
                {
                        car_frame_servo_motor.writeMicroseconds(i);
                        delay(SERVO_DELAY);
                }
        }
        if (!strcmp(bike_status, "RENT"))
        {
                for (int i = SERVO_MAX_PULSE_WIDTH; i > SERVO_MIN_PULSE_WIDTH; i-=10)
                {
                        car_frame_servo_motor.writeMicroseconds(i);
                        delay(SERVO_DELAY);
                }
        }
        if (!strcmp(bike_status, "WAITTING")) {}
}
