# Sistema de Empresas

## Arquivos

| Arquivo                    | O que é |
|-----------------------------|---------|
| `company.inc`               | Include mestre, atualizado para puxar os novos módulos. |
| `company_core.inc`          | Atualizado: categoria, dono, caixa, checagem de proximidade das pickups. |
| `company_dialogs.inc`       | Atualizado: `/criarempresa` sem dialogs de local, menu de `/editarempresa` com Categoria/Dono, e o bug do "Redefinir entrada" (dialog que nunca tinha sido implementado) corrigido. |
| `company_categories.inc`    | Categorias de empresa, modelos de pickup por categoria, itens pré-configurados e o dialog de compra (`/caixa`). |
| `company_commands.inc`      | Comandos avulsos: setar entrada/saída/ícone/dono, trancar/destrancar, arrombar, sacar caixa e gerenciar (adicionar/editar/remover) o checkpoint de vendas. |
| `companies.sql`             | Colunas (`map_icon_*`, `pickup_category`, `owner_id`, `cash`, `sales_*`) + tabela `company_purchases`. |

## Comandos (`company_commands.inc`)

Todos os comandos abaixo de configuração são **admin/moderador** (mesma flag de
`/criarempresa`). Os demais são de jogador comum.

| Comando | Quem usa | Descrição |
|---|---|---|
| `/empresasetentrada [id]` | Admin | Define a ENTRADA da empresa para a posição/interior atual. |
| `/empresasetsaida [id]` | Admin | Define a SAÍDA da empresa para a posição/interior atual. |
| `/empresaaddcheckpoint [id] [tamanho opcional]` | Admin | **ADICIONA** o checkpoint de **VENDAS** (separado de entrada/saída) na posição/interior atual — só funciona se a empresa ainda não tiver um. É ali que o `/caixa` fica disponível. |
| `/empresaeditarcheckpoint [id] [tamanho opcional]` | Admin | **EDITA** o checkpoint de vendas já existente: reposiciona para a posição/interior atual e, se informado, atualiza o tamanho (raio, em metros). Só funciona se a empresa já tiver um. |
| `/empresaremovercheckpoint [id]` | Admin | **REMOVE** o checkpoint de vendas da empresa (ela volta a não ter um até ser adicionado de novo). |
| `/empresaaddpickupgenerico [id] [modelo opcional]` | Admin | Adiciona um pickup **genérico** (sem função pré-definida) na posição atual. |
| `/empresaremoverpickupgenerico [id] [índice]` | Admin | Remove um pickup genérico pelo índice mostrado em `/empresalistarpickups`. |
| `/empresalistarpickups [id]` | Admin | Lista se o checkpoint de vendas está definido e quantos/quais pickups genéricos a empresa tem. |
| `/empresasetmapicone [id] [tipo] [corHex]` | Admin | Define o tipo (0-63) e a cor (ex.: `FFFFFFFF`) do ícone no mapa, e ativa o ícone. |
| `/empresasetpickup [id] [categoria]` | Admin | Define a categoria da empresa (0=Genérica, 1=Roupas, 2=24 Horas, 3=Restaurante, 4=Armaria). Isso muda o menu de itens do `/caixa` — entrada e saída sempre usam o modelo genérico de porta e não são afetadas. |
| `/empresasetdono [id] [playerid]` | Admin | Define o dono da empresa. |
| `/empresaremoverdono [id]` | Admin | Remove o dono (empresa volta a ser "do sistema"). |
| `/trancar` | Dono | Tranca a empresa. Precisa estar perto da pickup de entrada ou saída. |
| `/destrancar` | Dono | Destranca a empresa. Mesma exigência de proximidade. |
| `/arrombar` | Qualquer jogador | Tenta arrombar uma empresa trancada (não pode ser o dono). Leva 15s parado perto da pickup de entrada/saída; se sair do raio ou a empresa for destrancada, cancela. |
| `/caixa` | Qualquer jogador | Abre o menu de compras da empresa (baseado na categoria). Precisa estar dentro do **checkpoint de vendas**, com a empresa aberta/destrancada — abre também automaticamente ao entrar nesse checkpoint. |
| `/sacarcaixa [valor]` | Dono | Saca dinheiro do caixa acumulado da empresa (perto da entrada/saída). |

O mesmo pode ser feito pelo menu de `/editarempresa` → "Checkpoint de vendas", que
abre "Adicionar" (se ainda não existe) ou "Editar (reposicionar)" / "Remover" (se já
existe).

Os comandos de trancar/destrancar/arrombar/sacar usam **proximidade** com a pickup
de entrada/saída (`IsPlayerAtCompanyPickup`); o `/caixa` usa proximidade com o
checkpoint de **vendas** especificamente (`IsPlayerAtCompanySalesCheckpoint`), com o
raio do próprio checkpoint (`sales_size`, 3.0 metros por padrão). Os demais usam
`COMPANY_INTERACT_RADIUS` = 3.0 metros, editável em `company_core.inc`.
Isso porque `OnPlayerPickUpDynamicPickup` só dispara uma vez, no primeiro contato —
não dava pra usar isso pra saber se o jogador "ainda está" na pickup de entrada/saída.

## Pickups de entrada/saída, checkpoint de vendas e pickups genéricos

1. **Entrada/Saída** — continuam sendo **pickups**, sempre com o modelo genérico de
   porta (`COMPANY_PICKUP_MODEL`, `19135` por padrão), independente da categoria.
   Servem só para teleportar entre dentro/fora, e são o que `/trancar`, `/destrancar`
   e `/arrombar` verificam (via `OnPlayerPickUpDynamicPickup`).
2. **Vendas** — um único **checkpoint** por empresa, opcional (só existe depois de
   adicionado com `/empresaaddcheckpoint` ou pelo menu de `/editarempresa`). Pode ser
   adicionado, editado (posição e/ou tamanho) e removido — ver seção 3. É onde o
   `/caixa` funciona, e entrar nele já abre o menu de compra direto (via
   `OnPlayerEnterDynamicCP`, não precisa digitar `/caixa`).
3. **Genéricos** — de 0 a `MAX_COMPANY_GENERIC_PICKUPS` (10 por padrão) **pickups**
   por empresa, sem função pré-definida. Servem pra qualquer coisa que vocês queiram
   adicionar depois (uma garagem, uma banca de jornal, um caixa eletrônico, etc.) —
   tocar num deles dispara `OnCompanyGenericPickUp(playerid, companyid, index)`, que
   não faz nada por padrão. Implementem esse callback em outro módulo se quiserem
   dar uma função a eles.

Como pickups e checkpoints dinâmicos usam listas de ID separadas no streamer, o
checkpoint de vendas tem sua própria tabela de entidades (`s_CheckpointEntity`) e seu
próprio callback (`OnPlayerEnterDynamicCP`), independente de `s_PickupEntity` /
`OnPlayerPickUpDynamicPickup` usados pelas pickups de entrada/saída/genéricas. Se o
resto da sua gamemode também usa `OnPlayerEnterDynamicCP` para outra coisa, funda a
lógica manualmente (mesmo cuidado que já vale para `OnPlayerPickUpDynamicPickup`,
que este módulo também assume que é o único a definir).

Todos são recriados junto com o resto da empresa em `LoadCompanies`,
`CreateCompanyPickups`/`DestroyCompanyPickups`, e apagados junto no `DeleteCompany`
(os genéricos têm sua própria tabela, `company_pickups`, com `ON DELETE CASCADE`).

## Categorias e itens (`company_categories.inc`)

```pawn
enum e_COMPANY_CATEGORY {
    COMPANY_CAT_GENERICA = 0,
    COMPANY_CAT_ROUPAS,
    COMPANY_CAT_24HORAS,
    COMPANY_CAT_RESTAURANTE,
    COMPANY_CAT_ARMARIA,
    COMPANY_CAT_MAX
};
```

Cada categoria tem uma **lista de itens pré-configurada** (nome + preço, e no caso de
roupas também o ID da skin aplicada na compra). Isso é propositalmente simples/fixo
por enquanto — como combinado, no futuro dá pra trocar por itens configuráveis por
empresa vindos do banco (ex.: uma tabela `company_items`), sem mexer no resto do
sistema: baste trocar as funções `Company_GetCategoryItem*` para consultar o banco em
vez do array estático.

`g_CompanyCategoryPickupModel` (os IDs `19135`, `19194`, `19222`, `1247`) continua
declarado no arquivo, mas não é mais usado pelo checkpoint de vendas (que não tem
modelo/objeto 3D) — ele fica disponível caso vocês queiram usá-lo futuramente em
algum pickup genérico temático por categoria.

### Comprando (`/caixa`)

1. Jogador usa `/caixa` (ou entra no checkpoint) dentro do checkpoint de vendas de
   uma empresa aberta e destrancada.
2. Abre um `DIALOG_STYLE_LIST` com os itens da categoria da empresa.
3. Ao escolher um item: desconta o dinheiro do jogador (`GivePlayerMoney`), credita
   no caixa da empresa (`AddCompanyCash`), aplica o efeito (hoje só roupas, via
   `SetPlayerSkin`) e grava um registro em `company_purchases`.
4. Dispara `OnCompanyPurchase(playerid, companyid, itemName[], price)` — opcional,
   use `CallLocalFunction` no seu código se quiser reagir à compra (dar o item de
   verdade no inventário, etc.). Como não é `forward` direto, você não é obrigado a
   implementá-la.

## Dono, caixa e trancar/destrancar/arrombar

- `owner_id` na tabela é o "dono": por padrão o código usa
  `Company_GetPlayerOwnerID(playerid)`, que só chama `GetPlayerAccountID(playerid)`.
  **Ajuste essa função** em `company_commands.inc` se o seu sistema de contas usar
  outro nome (ex.: `GetPlayerCharacterID`), já que ela é chamada em um único lugar.
- `SetCompanyLocked` já existia no `company_core.inc` original; os comandos novos só
  chamam ela depois de validar dono + proximidade.
- `/arrombar` não mexe no banco a mais — só chama `SetCompanyLocked(companyid, false)`
  quando o tempo termina, igual ao `/destrancar`. Achei melhor não inventar uma
  coluna "arrombada" separada, já que pro sistema tanto faz *por que* está destrancada.
- O cancelamento do arrombamento ao desconectar **não é um auto-hook**. Chame
  manualmente na sua `OnPlayerDisconnect`:

```pawn
public OnPlayerDisconnect(playerid, reason)
{
    Company_HandleDisconnect(playerid);
    // ... resto do seu código
    return 1;
}
```

## Observações

- Ao criar a empresa, entrada e saída ficam **iguais** (a posição do admin no
  momento da criação) até serem ajustadas com os comandos. Não travei nada nisso —
  só é o valor inicial.
- O raio de interação (`COMPANY_INTERACT_RADIUS`) e o tempo de arrombamento
  (`COMPANY_BREAKIN_TIME`, 15s) são `#define` no topo dos respectivos arquivos,
  fáceis de ajustar. O tamanho padrão do checkpoint de vendas também é um `#define`
  (`COMPANY_SALES_CP_SIZE`, 3.0 metros) em `company_core.inc`, usado quando
  `/empresaaddcheckpoint`/`/empresaeditarcheckpoint` (ou o menu) não recebem um
  tamanho específico.
- `/empresaaddcheckpoint` só funciona se a empresa **ainda não** tiver um checkpoint
  de vendas, e `/empresaeditarcheckpoint` só funciona se ela **já** tiver — preferi
  deixar os dois comandos (e as opções do menu) claramente separados em vez de um
  único comando que decide sozinho entre "criar" ou "atualizar".
- `/arrombar` bloqueia o dono de arrombar a própria empresa (ele já tem
  `/destrancar`).
- O registro de compras (`company_purchases`) salva `buyer_id` como o `playerid`
  atual.
