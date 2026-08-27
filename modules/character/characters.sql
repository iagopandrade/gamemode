-- ---------------------------------------------------------------------
-- Tabela: characters
-- Responsavel exclusivamente pelos dados do PERSONAGEM. Uma conta
-- (accounts.id) pode possuir N personagens. Hoje o sistema utiliza
-- apenas 1 por conta, mas a estrutura ja suporta multiplos.
-- ---------------------------------------------------------------------

CREATE TABLE IF NOT EXISTS `characters` (
    `id`          INT UNSIGNED NOT NULL AUTO_INCREMENT,
    `account_id`  INT UNSIGNED NOT NULL COMMENT 'FK para accounts.id - dono do personagem',
    `cash`        INT NOT NULL DEFAULT 0,
    `score`       INT NOT NULL DEFAULT 0,
    `level`       INT UNSIGNED NOT NULL DEFAULT 0,
    `skin`        SMALLINT UNSIGNED NOT NULL DEFAULT 1,
    `interior`    SMALLINT UNSIGNED NOT NULL DEFAULT 0,
    `world`       INT UNSIGNED NOT NULL DEFAULT 0,
    `pos_x`       FLOAT NOT NULL DEFAULT 0,
    `pos_y`       FLOAT NOT NULL DEFAULT 0,
    `pos_z`       FLOAT NOT NULL DEFAULT 0,
    `pos_a`       FLOAT NOT NULL DEFAULT 0,
    `created_at`  DATETIME NOT NULL DEFAULT CURRENT_TIMESTAMP,
    PRIMARY KEY (`id`),
    KEY `idx_characters_account_id` (`account_id`),
    CONSTRAINT `fk_characters_account`
        FOREIGN KEY (`account_id`) REFERENCES `accounts` (`id`)
        ON DELETE CASCADE
        ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;
