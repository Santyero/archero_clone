# Integrantes
Santyero Mesquita Borges dos Santos

# Nome do Projeto

Archero Clone

# Breve Descrição

Jogo de geração de cenários e spawn de inimigos, onde o objetivo do jogador é desviar dos inimigos, projeteis e armadilhas do cenário jogado, sendo que o seu personagem atira automaticamente nos inimigos, porém só quando não está em movimento. Além disso, também conta com um sistema de evolução de nível do personagem, com que, a cada nível, é um possível escolher um power-up para ajudar a passar dos desafios.

# Complexidades

- Geração aleatória de cenário (deixado de lado por conta do tempo de desenvolvimento)
- Spawn de inimigos aleatórios
- Disparo automático do inimigo para o inimigo mais próximo
- Só disparar enquanto estiver parado
- Power-ups a subir de nível (deixado de lado por conta do tempo de desenvolvimento)
- Colisão com inimigos, projeteis e armadilhas

# Arquitetura a ser utilizada

- Arquitetura Baseada em Componentes

# Linguagem de Programação Utilizada

- C++

# Bibilotecas Utilizadas

= **SDL2**: https://www.libsdl.org/

# Jogo de Referência

**Archero**: https://play.google.com/store/apps/details?id=com.habby.archero&hl=pt_BR&gl=US

# Vídeo Demonstrativo do Jogo de Referência

- https://www.youtube.com/watch?v=4LRdw8LCCkU

# Sprites Utilizados

- **Tyny Swords**: https://pixelfrog-assets.itch.io/tiny-swords
- **generic-oldwest-pack**: https://bakudas.itch.io/generic-oldwest-pack
# Protótipo

https://www.figma.com/file/wAUblnHCA9l1T7gwmqFjPM/Archero-Clone?type=design&node-id=2%3A9&mode=design&t=3nY0EnRGzEhCgmUZ-1

# Adicionando variáveis do ambiente no Windows

setx SDL_INCLUDE_PATH "C:\caminho\para\sua\pasta\include\SDL"
setx SDL_LIB_PATH "C:\caminho\para\sua\pasta\lib\SDL"
