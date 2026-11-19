# 🅿️ Controle de Estacionamento

Sistema de controle de vagas de estacionamento desenvolvido para a A3. O projeto tem duas versões com a mesma lógica:

- **Programa em C** (`estacionamento.c`), que roda no terminal com um menu de opções;
- **Front-end web** (`front/`), em HTML, CSS e JavaScript, com mapa visual das vagas.

## Como funciona

O estacionamento tem **3 corredores (A, B e C) com 5 vagas cada**, um total de 15 vagas, guardadas em uma matriz `corredores x vagas`.

Quando um veículo entra, o sistema **compara os corredores** e escolhe o que tem **mais vagas livres**. O carro fica na primeira vaga livre desse corredor, o que distribui os veículos de forma equilibrada.

### Entradas

| Dado   | Descrição                                     |
|--------|-----------------------------------------------|
| Placa  | Identifica o veículo (convertida para maiúsculas) |
| Modelo | Modelo do carro, ex.: Gol                     |
| Cor    | Cor do carro, ex.: Prata                      |

### Saídas

| Informação             | Quando aparece                                              |
|------------------------|-------------------------------------------------------------|
| Vaga atribuída         | Na entrada: vaga onde o carro estacionou e horário (ex.: `B1 às 14:30`) |
| Tempo de permanência   | Na saída: quantos minutos o veículo ficou no estacionamento |

## Funcionalidades

- **Registrar entrada:** cadastra o veículo e escolhe a vaga automaticamente.
- **Registrar saída:** busca o carro pela placa, mostra o tempo de permanência e libera a vaga.
- **Mapa de vagas:** mostra quais vagas de cada corredor estão livres ou ocupadas.
- **Indicadores:** total de vagas, vagas livres e ocupadas, e percentual de ocupação.
- **Veículos estacionados:** lista os carros com vaga, placa, modelo, cor e horário de entrada.
- **Validações:** impede cadastrar a mesma placa duas vezes e avisa quando o estacionamento está lotado.

## Como executar

### Programa em C

É preciso ter o compilador **GCC** instalado.

```bash
gcc estacionamento.c -o estacionamento
./estacionamento
```

No Windows, o programa gerado é o `estacionamento.exe`.

Menu do programa:

```
===== CONTROLE DE ESTACIONAMENTO =====
1 - Registrar entrada
2 - Registrar saida
3 - Mapa de vagas
4 - Indicadores
5 - Veiculos estacionados
0 - Sair
```

### Front-end web

Não precisa instalar nada: basta abrir o arquivo `front/index.html` no navegador.

Na página é possível registrar entradas e saídas, ver os indicadores com uma barra de ocupação (verde, amarela ou vermelha conforme lota) e clicar em uma vaga ocupada para ver os dados do veículo.

> Os dados ficam só na memória: ao fechar o programa ou recarregar a página, as vagas voltam a ficar livres.

## Estrutura do projeto

```
├── estacionamento.c   # Programa principal em C
└── front/
    ├── index.html     # Página do sistema
    ├── style.css      # Estilos da página
    └── script.js      # Lógica do sistema em JavaScript
```

## Tecnologias

- C (bibliotecas `stdio.h`, `string.h`, `ctype.h` e `time.h`)
- HTML, CSS e JavaScript
