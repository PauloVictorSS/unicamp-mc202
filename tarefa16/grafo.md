**Quem são os vértices do gráfo?**

* Podemos considerar cada cela do livro sendo um vértice dentro de nosso gráfo, nesse sentido, cada vértice teria o nome da cela e os respectivos níveis mínimos de cada atributo para acessa-lá;

**Como serão representadas as arestas?**

* Com relação a arestas, vale ressaltar que podemos utilizar um grafo dirigido (ou digrafro), onde cada areasta/arco tem um ínicio e um fim, ou seja um sentido correto;
* Nessa representação, o vértice 'A' terá uma ligação no sentido do vértice 'B' quando a cela 'B' puder ser acessada pela cela 'A' em apenas uma rodada. Ou seja, o par  (A, B) indica que 'B' pode ser acessado por 'A' em uma rodada;
* Nessa representação, vale ressaltar, que não é porquê 'A' acessa 'B' em uma rodada, que 'B' acessa 'A' em uma rodada também. Não há uma dependência nessa representação;

**Como podemos pensar na saída do problema?**

* Depois que essas celas são definidas e todas as arestas/arcos estão bem definidos, podemos começar a pensar na resolução do problema;

* Sabendo que cada jogador começa em uma cela específica, podemos verificar, para cada jogador, quais são as celas que eles podem acessar em apenas uma rodada (somente olhando para nosso grafo). Comparando as celas que cada um pode acessar em uma rodada, sabemos quais são as celas em comum que todos os jogadores conseguem acessar em uma rodada (essas já vão para a resposta final);

* Seguindo um esquema parecido, para ver quais serão as possíveis celas de encontro para jogos de 2 rodadas, primeiramente, vemos quais são as celas que cada jogador acessa em uma rodada, depois, vemos quais celas, essas celas da primeira rodada, conseguem acessar, tentando achar celas em padrão (que farão parte da resposta, juntamente com as do tópico anterior);
