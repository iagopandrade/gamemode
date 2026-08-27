-- ---------------------------------------------------------------------
-- Tabela: accounts
-- Responsavel exclusivamente pelos dados da CONTA. Nenhum dado de
-- personagem (dinheiro, score, skin, posicao, etc) deve ser salvo aqui.
-- ---------------------------------------------------------------------

CREATE TABLE IF NOT EXISTS `accounts` (
    `id`          INT UNSIGNED NOT NULL AUTO_INCREMENT,
    `name`        VARCHAR(24)  NOT NULL COMMENT 'Nickname do jogador (login)',
    `password`    VARCHAR(129) NOT NULL COMMENT 'Hash Whirlpool da senha',
    `admin`       TINYINT UNSIGNED NOT NULL DEFAULT 0 COMMENT 'Flag/nivel de permissao administrativa',
    `last_ip`     VARCHAR(129) NULL DEFAULT NULL COMMENT 'Hash Whirlpool do ultimo IP usado',
    `created_at`  DATETIME NOT NULL DEFAULT CURRENT_TIMESTAMP,
    `last_login`  DATETIME NULL DEFAULT NULL,
    PRIMARY KEY (`id`),
    UNIQUE KEY `uq_accounts_name` (`name`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;
