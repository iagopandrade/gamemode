-- ---------------------------------------------------------------------
-- Tabela: login_blocks
-- Bloqueio de tentativas de login por IP (nunca por conta), evitando
-- que alguem bloqueie a conta de outro jogador apenas sabendo o nick.
-- ---------------------------------------------------------------------

CREATE TABLE IF NOT EXISTS `login_blocks` (
    `id`             INT UNSIGNED NOT NULL AUTO_INCREMENT,
    `ip_hash`        VARCHAR(129) NOT NULL COMMENT 'Hash Whirlpool do IP',
    `attempts`       INT UNSIGNED NOT NULL DEFAULT 0,
    `blocked_until`  INT UNSIGNED NOT NULL DEFAULT 0 COMMENT 'Unix timestamp; 0 = nao bloqueado',
    `created_at`     DATETIME NOT NULL DEFAULT CURRENT_TIMESTAMP,
    `updated_at`     DATETIME NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
    PRIMARY KEY (`id`),
    UNIQUE KEY `uq_login_blocks_ip_hash` (`ip_hash`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;
