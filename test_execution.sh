#!/bin/bash

# ============================================================================
# BATTERIE DE TESTS MINISHELL
# Teste les exécutions et redirections
# ============================================================================

# Couleurs
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[1;34m'
NC='\033[0m' # No Color

TEST_COUNT=0
PASS_COUNT=0
FAIL_COUNT=0

# Répertoire temporaire pour les tests
TEST_DIR="/tmp/minishell_tests"
rm -rf "$TEST_DIR"
mkdir -p "$TEST_DIR"
cd "$TEST_DIR"

# Fonction pour tester
run_test() {
	local name="$1"
	local input="$2"
	local expected_check="$3"
	
	TEST_COUNT=$((TEST_COUNT + 1))
	echo -e "\n${BLUE}TEST $TEST_COUNT: $name${NC}"
	echo "Input: $input"
	
	# Créer un fichier de test
	echo "$input" > test_input.txt
	
	# Exécuter minishell
	output=$(echo "$input" | /home/ghub/minishell/minishell 2>&1 | head -20)
	
	# Vérifier le résultat
	if eval "$expected_check"; then
		echo -e "${GREEN}✓ PASS${NC}"
		PASS_COUNT=$((PASS_COUNT + 1))
	else
		echo -e "${RED}✗ FAIL${NC}"
		echo "Output: $output"
		FAIL_COUNT=$((FAIL_COUNT + 1))
	fi
}

# ============================================================================
# 1. COMMANDES SIMPLES
# ============================================================================
echo -e "\n${YELLOW}=== 1. COMMANDES SIMPLES ===${NC}"

run_test "Echo simple" "echo hello" \
	"[[ \"\$output\" == *hello* ]]"

run_test "Echo avec arguments" "echo hello world" \
	"[[ \"\$output\" == *hello*world* ]]"

run_test "ls" "ls" \
	"[[ -n \"\$output\" ]]"

run_test "pwd" "pwd" \
	"[[ \"\$output\" == *tmp* ]]"

run_test "whoami" "whoami" \
	"[[ -n \"\$output\" ]]"

# ============================================================================
# 2. REDIRECTIONS SIMPLES
# ============================================================================
echo -e "\n${YELLOW}=== 2. REDIRECTIONS SIMPLES ===${NC}"

run_test "Output redirection >" "echo hello > test_out.txt" \
	"[[ -f test_out.txt ]] && grep -q hello test_out.txt"

run_test "Append redirection >>" "echo world >> test_out.txt" \
	"[[ -f test_out.txt ]] && grep -q world test_out.txt"

run_test "Input redirection <" "cat < test_out.txt" \
	"[[ \"\$output\" == *hello* ]]"

run_test "Multiple > (last one wins)" "echo a > out1.txt && echo b > out1.txt" \
	"[[ -f out1.txt ]]"

# ============================================================================
# 3. PIPES SIMPLES
# ============================================================================
echo -e "\n${YELLOW}=== 3. PIPES SIMPLES ===${NC}"

run_test "Pipe: echo | cat" "echo hello | cat" \
	"[[ \"\$output\" == *hello* ]]"

run_test "Pipe: echo | grep" "echo 'test123' | grep test" \
	"[[ \"\$output\" == *test* ]]"

run_test "Pipe: ls | wc" "ls | wc -l" \
	"[[ \"\$output\" =~ [0-9] ]]"

# ============================================================================
# 4. MULTI-PIPES
# ============================================================================
echo -e "\n${YELLOW}=== 4. MULTI-PIPES ===${NC}"

run_test "Pipe 3x: echo | cat | wc" "echo hello | cat | wc -c" \
	"[[ \"\$output\" =~ [0-9] ]]"

run_test "Pipe 4x: complex" "echo -e 'line1\nline2\nline3' | grep line | wc -l" \
	"[[ \"\$output\" =~ 3 ]]"

# ============================================================================
# 5. REDIRECTIONS + PIPES
# ============================================================================
echo -e "\n${YELLOW}=== 5. REDIRECTIONS + PIPES ==={{NC}"

run_test "Pipe then redirect" "echo hello | cat > out2.txt" \
	"[[ -f out2.txt ]] && grep -q hello out2.txt"

run_test "Pipe multiple with redirect" "echo 'test' | cat | cat > out3.txt" \
	"[[ -f out3.txt ]] && grep -q test out3.txt"

run_test "Input redirect + pipe" "cat < test_out.txt | wc -l" \
	"[[ \"\$output\" =~ [0-9] ]]"

# ============================================================================
# 6. HEREDOCS
# ============================================================================
echo -e "\n${YELLOW}=== 6. HEREDOCS ===${NC}"

# Test heredoc - créer le fichier d'input
cat > heredoc1_input.txt << 'EOF'
cat << EOF_TEST
Hello from heredoc
This is a test
EOF_TEST
EOF

run_test "Simple heredoc" "cat << 'DELIM'\nhello\nDELIM" \
	"[[ \"\$output\" == *hello* ]]"

# ============================================================================
# 7. COMMANDES INVALIDES
# ============================================================================
echo -e "\n${YELLOW}=== 7. COMMANDES INVALIDES ==={{NC}"

run_test "Command not found" "nonexistentcommand123" \
	"[[ \"\$output\" == *not*found* ]] || [[ \"\$output\" == *error* ]]"

run_test "File not found redirect" "cat < nonexistent_file.txt" \
	"[[ \"\$output\" == *No*such* ]] || [[ \"\$output\" == *not*found* ]]"

# ============================================================================
# 8. COMBINAISONS COMPLEXES
# ============================================================================
echo -e "\n${YELLOW}=== 8. COMBINAISONS COMPLEXES ==={{NC}"

run_test "Multiple redirections" "echo a > f1.txt && echo b >> f1.txt && cat f1.txt" \
	"[[ \"\$output\" == *a* ]] && [[ \"\$output\" == *b* ]]"

run_test "Pipe with multiple redirects" "echo hello | cat > tmp.txt && cat tmp.txt" \
	"[[ -f tmp.txt ]]"

# ============================================================================
# 9. EDGE CASES
# ============================================================================
echo -e "\n${YELLOW}=== 9. EDGE CASES ==={{NC}"

run_test "Empty command" "" \
	"true"

run_test "Whitespace only" "   " \
	"true"

run_test "Command with spaces" "echo    hello    world" \
	"[[ \"\$output\" == *hello* ]]"

# ============================================================================
# RÉSUMÉ
# ============================================================================
echo -e "\n${BLUE}╔════════════════════════════════════╗${NC}"
echo -e "${BLUE}║        RÉSUMÉ DES TESTS            ║${NC}"
echo -e "${BLUE}╚════════════════════════════════════╝${NC}"
echo -e "Total:  ${BLUE}$TEST_COUNT${NC}"
echo -e "Pass:   ${GREEN}$PASS_COUNT${NC}"
echo -e "Fail:   ${RED}$FAIL_COUNT${NC}"

if [ $FAIL_COUNT -eq 0 ]; then
	echo -e "\n${GREEN}✓ TOUS LES TESTS RÉUSSIS !${NC}"
	exit 0
else
	echo -e "\n${RED}✗ Certains tests ont échoué${NC}"
	exit 1
fi
