# Automacao de Piscina com Arduino

### Projeto de automacao com arduino

<img src="https://github.com/Gabrirodri/Automacao-arduino-piscina/blob/Gabrirodri-patch-2/imagens/painel%20automacao%20piscina.jpeg" width="300">        <img src="https://github.com/Gabrirodri/Automacao-arduino-piscina/blob/Gabrirodri-patch-2/imagens/painel%20automacao%20piscina%202.jpeg" width="190">

## Faz medição de PH e Temperatura da Agua

Dentro das condições de PH Alto, Baixo, ativa portas na saida para ligar relé, podendo ligar um sinalizador ou acionar uma bomba...
As condições de Temperatura, podem ser  editados no programa, se for abaixo da temperatura indicada, liga uma porta na saida, podendo acionar um relé e possivelmente um aquecedor.

<img src="https://github.com/Gabrirodri/Automacao-arduino-piscina/blob/master/imagens/circuito-fritizing.png" width="350">

Possui um *Timer* que fica em um ciclo de loop em paralelo com outras funções do programa, acionando uma porta para filtração e cloração... 
O tempo que vão fica ligadas pode ser editado, alterando o tempo do timer. 

Todo o Codigo esta comentado, o Circuito foi desenhado no Software Fritizing
Para compilar o codigo é necessario instalar as bibliotecas, dentro da pasta: _Documents\Arduino\libraries_

O sensor de temperatura usado é o DS18B20
O programa possui tambem um relogio, com data e hora que são contados no LCD
Depois de um tempo na tela do relogio, muda para outra tela mostrando o valor de Temperatura, PH, e tambem o Status do PH(Se esta baixo, ideal ou alto)
