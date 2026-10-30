# Avaliação Prática — Módulo 2 - baremetal

- Você pode consultar qualquer material próprio ou fornecido ao longo do curso.
- Duração máxima: **2 horas**.
- **Não é permitido utilizar LLMs** (ChatGPT, Copilot Chat, etc.).
- O **SMOWL deve permanecer aberto durante toda a avaliação**.

> [!IMPORTANT]
> Após concluir a atividade, siga exatamente as mesmas etapas do laboratório prático (fluxo do PrairieLearn):
> 
> - Validar a qualidade do código (GitHub Actions)
> - Solicitar avaliação manual (durante o período da avaliação)

## Descrição da Avaliação

> Periféricos necessários:
> - 1x botão
> - UART

Implemente um sistema de acionamento temporizado utilizando um botão e um LED. O botão solicita que o estado estável do LED seja alterado, mas a alteração somente deve ser confirmada após **6 segundos**. Durante esse intervalo, o LED deve piscar para indicar que existe uma ação pendente.

O sistema deve iniciar com o LED apagado e apresentar o seguinte comportamento:

### Acionamento do LED

- Com o LED apagado, um acionamento do botão deve iniciar uma temporização de **6 segundos**.
- Durante a temporização, o LED deve piscar progressivamente, começando em uma frequência baixa e aumentando gradualmente a frequência.
- Ao final dos 6 segundos, a ação deve ser confirmada e o LED deve permanecer aceso continuamente.

### Desligamento do LED

- Com o LED aceso, um acionamento do botão deve iniciar uma nova temporização de **6 segundos**.
- Durante a temporização, o LED deve piscar, começando em uma frequência alta e reduzindo gradualmente a frequência.
- Ao final dos 6 segundos, a ação deve ser confirmada e o LED deve permanecer apagado.

### Cancelamento

Se o botão for acionado enquanto o LED estiver piscando, a ação pendente deve ser cancelada imediatamente. Nesse caso, o LED deve retornar ao estado estável anterior ao início da temporização:

- se o sistema estava iniciando o acionamento, o LED deve voltar a permanecer apagado;
- se o sistema estava iniciando o desligamento, o LED deve voltar a permanecer aceso.

Após uma confirmação ou um cancelamento, o sistema deve estar pronto para receber um novo acionamento do botão. A leitura do botão e o controle da temporização e das piscadas não devem bloquear a execução do restante do sistema.

### Diagramas temporais

Cada pulso na linha `BOTÃO` representa um acionamento. Os intervalos entre as piscadas são apenas ilustrativos; o importante é que a frequência varie gradualmente ao longo dos 6 segundos.

#### LED apagado — acionamento confirmado

```text
EVENTO                solicitação                         confirmação (6s)
BOTÃO      ___________|‾|_____________________________________________
LED        ___________|‾‾‾‾|____|‾‾‾|___|‾‾|__|‾|_|‾|_|‾|_|‾‾‾‾‾‾‾‾
ESTADO       APAGADO          frequência crescente                ACESO
```

#### LED apagado — acionamento cancelado

```text
EVENTO                solicitação            cancelamento
BOTÃO      ___________|‾|________________________|‾|__________________
LED        ___________|‾‾‾‾|____|‾‾‾|___|‾‾|__|‾|_|‾|__________________
ESTADO       APAGADO       frequência crescente          APAGADO
```

O segundo acionamento interrompe a temporização, cancela a ação pendente e restaura imediatamente o estado apagado.

#### LED aceso — desligamento confirmado

```text
EVENTO                solicitação                         confirmação (6s)
BOTÃO      ___________|‾|_____________________________________________
LED        ‾‾‾‾‾‾‾‾‾‾|‾|_|‾|_|‾‾|__|‾‾‾|___|‾‾‾‾|______|‾‾‾‾‾‾‾‾|___________
ESTADO        ACESO           frequência decrescente             APAGADO
```

## Regras

- **Proibido**: realizar polling no GPIO: `gpio_get`
- O código deve passar no `cppcheck` e no `embedded check`
- Demonstrar todas as funcionalidades no hardware real
