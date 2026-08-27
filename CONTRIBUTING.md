# 🤝 Contribuindo com o BPC

Toda contribuição é bem-vinda! Este guia explica como preparar o ambiente e enviar suas mudanças.

---

## ⚠️ Antes de começar

Este repositório contém **apenas** o código-fonte do gamemode. Ele **não** inclui o compilador nem as dependências (plugins/includes) prontas para uso.

Para compilar e contribuir, você **precisa baixar o [último Release](https://github.com/BPC-Players-Community/gamemode/releases)** — ele já vem com o compilador, os includes e os plugins configurados. Clonar só a source não é suficiente para compilar o projeto.

---

## 🚀 Compilando

### 1. Baixe o Release

Acesse a página de [Releases](https://github.com/BPC-Players-Community/gamemode/releases) e baixe o pacote mais recente. Ele contém:

- Compilador do Pawn já configurado
- Todas as includes necessárias
- Plugins pré-instalados na pasta `plugins`

Extraia o conteúdo em uma pasta local — essa será sua base de desenvolvimento.

### 2. Atualize a source

O código-fonte (`.pwn` / `.inc`) evolui neste repositório. Para trabalhar com a versão mais recente:

```bash
git clone https://github.com/BPC-Players-Community/gamemode.git
```

Copie os arquivos de source clonados para dentro da pasta do Release baixado, substituindo os arquivos correspondentes.

### 3. Compile

Abra a pasta (a do Release, já com a source atualizada) no VSCode e compile normalmente.

---

## 🛠 Dependências

Esta gamemode utiliza as seguintes bibliotecas e componentes. **Todas já vêm prontas no Release** — a lista abaixo é só para referência, você não precisa baixar nada manualmente.

### 📦 Includes

* open.mp
* PawnPlus
* Pawn.CMD
* pp-hooks
* y_unique
* streamer
* easyDialog
* sscanf2
* foreach
* a_mysql
* whirlpool
* WeatherSystem
* gametext_plus
* mSelection

### 🔌 Plugins

* **streamer** — https://github.com/samp-incognito/samp-streamer-plugin/releases
* **sscanf2** — https://github.com/Y-Less/sscanf/releases
* **whirlpool** — https://github.com/Southclaws/samp-whirlpool/releases
* **PawnPlus** — https://github.com/IS4Code/PawnPlus/releases

### ⚙️ Components

* **Pawn.CMD** — https://github.com/katursis/Pawn.CMD/releases

---

## 🔀 Fluxo de contribuição

1. Faça um Fork
2. Crie uma Branch
3. Faça commits descritivos

4. Abra um Pull Request.

---

## 📋 Padrão de Código

Ainda não foi definido, mas a maioria dos sistemas seguem o guia de estilo do open.mp:https://open.mp/docs/scripting/language/Style
