
#include <WiFi.h>

#define Rele1 0
#define Rele2 15
 
bool Estado_Do_Rele1 = LOW;
bool Estado_Do_Rele2 = LOW;

// substitua os dados entre aspas com o nome da Rede e a senha desejados
const char* ssid = "SSID da sua rede";
const char* password = "Senha da sua rede";

WiFiServer server(80);
 
void setup() {
  Serial.begin(115200);
  pinMode(Rele1, OUTPUT);
  pinMode(Rele2, OUTPUT);
 
  Serial.println();
  Serial.print("Conectando-se a ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
 
  Serial.println("");
  Serial.println("WiFi conectada.");
  Serial.println("Endereço de IP: ");
  Serial.println(WiFi.localIP());
 
  server.begin();
}
 
void loop() {
  WiFiClient client = server.available(); // verifica e armazena se tem clientes conectados na rede
  if (client) { // se você conseguir um cliente,
    Serial.println("Novo Cliente."); // imprime uma mensagem pela porta serial
    String currentLine = ""; // faz uma String para manter os dados recebidos do cliente
    while (client.connected()) { // loop enquanto o cliente está conectado
      if (client.available()) { // se houver bytes para ler do cliente,
        char c = client.read(); // leia um byte e depois
        Serial.write(c); // imprima o monitor serial
        if (c == '\n') { // se o byte for um caractere de nova linha
        /* se a linha atual estiver em branco, você terá dois caracteres de nova linha em uma linha.
esse é o fim da solicitação HTTP do cliente, então envie uma resposta: */
          if (currentLine.length() == 0) {
            /* Os cabeçalhos HTTP sempre começam com um código de resposta (por exemplo, HTTP / 1.1 200 OK)
e um tipo de conteúdo para que o cliente saiba o que está por vir e, em seguida, uma linha em branco: */
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();
            
            // configuração de estilo do site
            client.print("<style type=\"text/css\"> a{margin: 50px 50px; background: #000000; color: #ffffff; text-decoration: none; padding: 1% 20% 1%; border-radius: 10px; font-size: 8.0em;}</style>");
            // o conteúdo do cabeçalho HTTP
            client.print("<a href=\"/R1\">Rele 1. </a> </br></br></br>");
            client.print("<a href=\"/R2\">Rele 2. </a> </br></br></br>");
            
            // A resposta HTTP termina com outra linha em branco:
            client.println();
            // sair do loop while:
            break;
          } else { // se você tiver uma nova linha, então limpe a linha atual:
            currentLine = "";
          }
        } else if (c != '\r') { // se você tiver qualquer outra coisa além de um caractere de retorno de linha,
          currentLine += c; // adicioná-lo ao final da linha atual
        }
        if (currentLine.endsWith("GET /R1")) {
          if (Estado_Do_Rele1 == LOW) {
            digitalWrite(Rele1, HIGH);
            Estado_Do_Rele1 = HIGH;
          } 
          else {
            digitalWrite(Rele1, LOW);
            Estado_Do_Rele1 = LOW;
          }
        }
        if (currentLine.endsWith("GET /R2")) {      
          if (Estado_Do_Rele2 == LOW) {
            digitalWrite(Rele2, HIGH);
            Estado_Do_Rele2 = HIGH;
          }
          else {
            digitalWrite(Rele2, LOW);
            Estado_Do_Rele2 = LOW;
          }
        }
      }
    }
    client.stop();
    Serial.println("Cliente Desconectado.");
  }
}
