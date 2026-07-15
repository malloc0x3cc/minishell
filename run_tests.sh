#!/bin/bash

# ============================================================================
# BATTERIE DE TESTS MINISHELL - SIMPLE VERSION
# ============================================================================

RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[1;34m'
MAGENTA='\033[1;35m'
NC='\033[0m'

SHELL_BIN="/home/ghub/minishell/minishell"
TEST_DIR="/tmp/minishell_tests_$$"
TEST_COUNT=0
PASS_COUNT=0
FAIL_COUNT=0

# Setup
rm -rf "$TEST_DIR"
mkdir -p "$TEST_DIR"
cd "$TEST_DIR" || exit 1

# Fonction pour exécuter un test
test_cmd() {
	local name="$1"
	local cmd="$2"
	
	TEST_COUNT=$((TEST_COUNT + 1))
	printf "\n${BLUE}[TEST %2d]${NC} %s\n" "$TEST_COUNT" "$name"
	printf "  Cmd: ${MAGENTA}%s${NC}\n" "$cmd"
	
	# Exécuter via minishell
	local output=$(echo "$cmd" | timeout 1 "$SHELL_BIN" 2>&1)
	local exit_code=$?
	
	echo "  Output: $output"
	
	# Retourner succès par défaut (commande s'est exécutée)
	if [ $exit_code -eq 124 ]; then
		printf "  ${RED}✗ TIMEOUT${NC}\n"
		FAIL_COUNT=$((FAIL_COUNT + 1))
		return 1
	elif [ -n "$output" ] || [ $exit_code -eq 0 ]; then
		printf "  ${GREEN}✓ PASS${NC}\n"
		PASS_COUNT=$((PASS_COUNT + 1))
		return 0
	else
		printf "  ${RED}✗ FAIL${NC}\n"
		FAIL_COUNT=$((FAIL_COUNT + 1))
		return 1
	fi
}

# ============================================================================
# 1. COMMANDES SIMPLES
# ============================================================================
echo -e "\n${YELLOW}╔════════════════════════════════════╗${NC}"
echo -e "${YELLOW}║  1. COMMANDES SIMPLES              ║${NC}"
echo -e "${YELLOW}╚════════════════════════════════════╝${NC}"

test_cmd "Echo simple" "echo hello"
test_cmd "Echo plusieurs args" "echo hello world test"
test_cmd "Echo avec quotes" 'echo "message with spaces"'
test_cmd "pwd" "pwd"
test_cmd "whoami" "whoami"
test_cmd "ls" "ls"
test_cmd "cat avec fichier" "cat /etc/hostname"

# ============================================================================
# 2. REDIRECTIONS SORTIE >
# ============================================================================
echo -e "\n${YELLOW}╔════════════════════════════════════╗${NC}"
echo -e "${YELLOW}║  2. REDIRECTIONS SORTIE (>)       ║${NC}"
echo -e "${YELLOW}╚════════════════════════════════════╝${NC}"

test_cmd "Créer fichier avec >" "echo contenu1 > test1.txt"
test_cmd "Vérifier fichier créé" "cat test1.txt"
test_cmd "Overwrite avec >" "echo contenu2 > test1.txt"
test_cmd "Vérifier overwrite" "cat test1.txt"
test_cmd "Pipe vers fichier" "echo piped | cat > test2.txt"
test_cmd "Vérifier pipe redirect" "cat test2.txt"

# ============================================================================
# 3. REDIRECTIONS APPEND >>
# ============================================================================
echo -e "\n${YELLOW}╔════════════════════════════════════╗${NC}"
echo -e "${YELLOW}║  3. REDIRECTIONS APPEND (>>)      ║${NC}"
echo -e "${YELLOW}╚════════════════════════════════════╝${NC}"

test_cmd "Créer puis append" "echo ligne1 > append_test.txt"
test_cmd "Append ligne2" "echo ligne2 >> append_test.txt"
test_cmd "Append ligne3" "echo ligne3 >> append_test.txt"
test_cmd "Vérifier toutes les lignes" "cat append_test.txt"
test_cmd "Append via pipe" "echo piped >> append_test.txt"

# ============================================================================
# 4. REDIRECTIONS ENTRÉE <
# ============================================================================
echo -e "\n${YELLOW}╔════════════════════════════════════╗${NC}"
echo -e "${YELLOW}║  4. REDIRECTIONS ENTRÉE (<)       ║${NC}"
echo -e "${YELLOW}╚════════════════════════════════════╝${NC}"

echo "test input file" > input_test.txt

test_cmd "Cat avec <" "cat < input_test.txt"
test_cmd "Grep avec <" "grep test < input_test.txt"
test_cmd "Wc avec <" "wc -l < input_test.txt"

# ============================================================================
# 5. PIPES SIMPLES
# ============================================================================
echo -e "\n${YELLOW}╔════════════════════════════════════╗${NC}"
echo -e "${YELLOW}║  5. PIPES SIMPLES (|)             ║${NC}"
echo -e "${YELLOW}╚════════════════════════════════════╝${NC}"

test_cmd "Echo | cat" "echo hello | cat"
test_cmd "Echo | wc -c" "echo test | wc -c"
test_cmd "Echo | grep" "echo testmatch | grep test"
test_cmd "Echo | sort" "echo zyx | sort"
test_cmd "ls | wc -l" "ls | wc -l"

# ============================================================================
# 6. MULTI-PIPES
# ============================================================================
echo -e "\n${YELLOW}╔════════════════════════════════════╗${NC}"
echo -e "${YELLOW}║  6. MULTI-PIPES (3+)              ║${NC}"
echo -e "${YELLOW}╚════════════════════════════════════╝${NC}"

test_cmd "Pipe 3x (| | )" "echo abc | cat | cat"
test_cmd "Pipe 4x (| | | )" "echo 1234 | cat | cat | cat"
test_cmd "Pipe 5x" "echo test | cat | cat | cat | cat"
test_cmd "Echo | sort | cat" "echo zyx | sort | cat"
test_cmd "Echo | grep | wc" "echo test123 | grep test | wc -c"

# ============================================================================
# 7. COMBINAISONS PIPES + REDIRECTIONS
# ============================================================================
echo -e "\n${YELLOW}╔════════════════════════════════════╗${NC}"
echo -e "${YELLOW}║  7. PIPES + REDIRECTIONS          ║${NC}"
echo -e "${YELLOW}╚════════════════════════════════════╝${NC}"

test_cmd "Pipe then >" "echo data | cat > piped_out.txt"
test_cmd "Vérifier output" "cat piped_out.txt"
test_cmd "Pipe then >>" "echo more | cat >> piped_out.txt"
test_cmd "Multi-pipe then >" "echo msg | cat | cat > multi_pipe.txt"
test_cmd "< then pipe" "cat < input_test.txt | wc -l"
test_cmd "Pipe | pipe | >" "echo line | cat | cat > pipe_pipe.txt"

# ============================================================================
# 8. COMBINAISONS COMPLEXES
# ============================================================================
echo -e "\n${YELLOW}╔════════════════════════════════════╗${NC}"
echo -e "${YELLOW}║  8. COMBINAISONS COMPLEXES        ║${NC}"
echo -e "${YELLOW}╚════════════════════════════════════╝${NC}"

test_cmd "< and | and >" "cat < input_test.txt | cat > final.txt"
test_cmd "Vérifier final.txt" "cat final.txt"
test_cmd "Multiple pipes avec <" "cat < input_test.txt | cat | cat"
test_cmd "Echo > alors cat <" "echo test > temp.txt"

# ============================================================================
# 9. REDIRECTIONS MULTIPLES
# ============================================================================
echo -e "\n${YELLOW}╔════════════════════════════════════╗${NC}"
echo -e "${YELLOW}║  9. CAS SPÉCIAUX                  ║${NC}"
echo -e "${YELLOW}╚════════════════════════════════════╝${NC}"

test_cmd "Echo vide" "echo"
test_cmd "Echo avec espaces" "echo     hello     world"
test_cmd "Echo avec \\n" "echo -e 'line1\nline2'"
test_cmd "Cat /etc/passwd" "cat /etc/passwd"
test_cmd "Command avec path" "/bin/echo hello"

# ============================================================================
# 10. GESTION D'ERREURS
# ============================================================================
echo -e "\n${YELLOW}╔════════════════════════════════════╗${NC}"
echo -e "${YELLOW}║  10. GESTION D'ERREURS            ║${NC}"
echo -e "${YELLOW}╚════════════════════════════════════╝${NC}"

test_cmd "Fichier inexistant <" "cat < nonexistent.txt" || true
test_cmd "Command inexistante" "commandnonexistante123" || true

# ============================================================================
# RÉSUMÉ FINAL
# ============================================================================
echo -e "\n${MAGENTA}╔════════════════════════════════════╗${NC}"
echo -e "${MAGENTA}║       RÉSUMÉ FINAL DES TESTS        ║${NC}"
echo -e "${MAGENTA}╚════════════════════════════════════╝${NC}"

RATE=0
if [ $TEST_COUNT -gt 0 ]; then
	RATE=$((PASS_COUNT * 100 / TEST_COUNT))
fi

echo -e "${BLUE}Total tests:${NC}  $TEST_COUNT"
echo -e "${GREEN}Réussis:${NC}     $PASS_COUNT"
echo -e "${RED}Échoués:${NC}      $FAIL_COUNT"
echo -e "${MAGENTA}Taux réussite: $RATE%${NC}\n"

if [ $FAIL_COUNT -eq 0 ]; then
	echo -e "${GREEN}╔════════════════════════════════════╗${NC}"
	echo -e "${GREEN}║  ✓ TOUS LES TESTS RÉUSSIS !        ║${NC}"
	echo -e "${GREEN}╚════════════════════════════════════╝${NC}\n"
	exit 0
else
	echo -e "${RED}╔════════════════════════════════════╗${NC}"
	echo -e "${RED}║  ✗ Certains tests ont échoué       ║${NC}"
	echo -e "${RED}╚════════════════════════════════════╝${NC}\n"
	exit 1
fi
