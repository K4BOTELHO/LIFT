# 📺 TV Lift Automático com Arduino Nano

Este projeto implementa um **sistema de automação para levantamento e recolhimento de TVs**, ideal para uso em móveis planejados e ambientes com foco em design, tecnologia e sofisticação. Desenvolvido com **Arduino Nano**, sensores de fim de curso e controle via **RF e IR**, o projeto proporciona uma solução robusta e elegante para automação residencial.

---

## ✨ Destaques do Projeto

- Controle por **controle remoto RF 433MHz**.
- Compatível com **controles IR** (como controles universais).
- Interface de comando via **Serial/USB** para testes e integração.
- **Sensores de fim de curso** para proteção e parada automática.
- **Temporização inteligente** para segurança do motor.
- Acionamento de **freio eletromecânico** para travamento preciso.

---

## 🛠️ Requisitos de Hardware

- 🔌 **Arduino Nano** (ATmega328P)
- 📡 **Módulo RF 433MHz transmissor/receptor**
- 🎮 **Controle remoto RF virgem programado**
- 📥 **Receptor IR**
- 🧠 **Módulo ou sistema de acionamento de motor DC / Atuador linear**
- 🔒 **Freio Eletromecânico** (opcional, mas recomendado)
- ⚙️ **2 Fins de curso** (para limites superior e inferior)
- 💡 LED de status (opcional)
- Fonte de alimentação de acordo com o motor utilizado

---

## ⚙️ Funcionalidade

O sistema é responsável por movimentar uma TV de dentro de um móvel com **total precisão e segurança**, trazendo um toque moderno e automatizado ao ambiente.  
Suporta três modos de acionamento:

- 📶 **RF:** Controle remoto físico para subir, descer e parar.
- 📲 **IR:** Controle infravermelho (universal ou dedicado).
- 💻 **Serial:** Comandos via monitor serial para testes ou integração com automações maiores.

---

## 🔌 Pinos Utilizados no Arduino

| Função              | Pino |
|---------------------|------|
| Subir Motor         | 4    |
| Descer Motor        | 11   |
| Freio Motor         | 17   |
| Sensor Abertura     | 5    |
| Sensor Fechamento   | 10   |
| Receptor IR         | 16   |
| RF Receiver         | 2    *(interrupt 0)* |

---

## 🧠 Lógica de Segurança

- **Fim de curso** superior e inferior garantem parada exata.
- **Delay programado** entre liberação de freio e ativação do motor.
- **Parada forçada após 20 segundos** se nenhum sensor for ativado.
- Comandos ignoram reativações rápidas para evitar falhas mecânicas.

---

## 👨‍💻 Autor
- `Base desenvolvida por K4BOTELHO`

---

## 📜 Licença

Este repositório está sob a licença MIT, permitindo uso livre pessoal ou comercial.

---

## ⭐ Contribua

Se este projeto foi útil para você, deixe uma ⭐ estrela no repositório. Isso ajuda a fortalecer a comunidade e o alcance do projeto.

---

