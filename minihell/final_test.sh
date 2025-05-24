#!/bin/bash

echo "=== COMPREHENSIVE TEST: Verifying heredoc timing fix ==="
echo ""

# Test 1: Complete heredoc followed by incomplete heredoc
echo "Test 1: ls | << h (with content) followed by wc | <<"
echo "Expected: Should read heredoc first, then show syntax error"
echo ""

echo "=== BASH OUTPUT ==="
cat > /tmp/test1_input << 'EOF'
ls | << h
heredoc content line 1
heredoc content line 2
h
wc | <<
EOF

echo "Input:"
cat /tmp/test1_input
echo ""
echo "Bash result:"
bash < /tmp/test1_input 2>&1
echo ""

echo "=== MINISHELL OUTPUT ==="
echo "Minishell result:"
./minishell < /tmp/test1_input 2>&1
echo ""

# Test 2: Just incomplete heredoc
echo "======================================="
echo "Test 2: Just incomplete heredoc (wc | <<)"
echo ""

echo "=== BASH OUTPUT ==="
echo 'wc | <<' | bash 2>&1
echo ""

echo "=== MINISHELL OUTPUT ==="
echo 'wc | <<' | ./minishell 2>&1
echo ""

# Test 3: Multiple incomplete heredocs
echo "======================================="
echo "Test 3: Multiple operations with incomplete heredocs"
echo ""

cat > /tmp/test3_input << 'EOF'
echo hello | << del1
content1
del1
cat <<
EOF

echo "=== BASH OUTPUT ==="
echo "Input:"
cat /tmp/test3_input
echo ""
echo "Bash result:"
bash < /tmp/test3_input 2>&1
echo ""

echo "=== MINISHELL OUTPUT ==="
echo "Minishell result:"
./minishell < /tmp/test3_input 2>&1

# Cleanup
rm -f /tmp/test1_input /tmp/test3_input

echo ""
echo "=== TEST SUMMARY ==="
echo "The fix should ensure that:"
echo "1. Heredocs are processed (read) before syntax errors are checked"
echo "2. Incomplete heredocs (like 'wc | <<') show syntax errors after any complete heredocs are processed"
echo "3. The behavior matches bash as closely as possible"
