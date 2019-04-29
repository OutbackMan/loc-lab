set nocompatible

set fileencoding=utf-8 encoding=utf-8
set noswapfile noundofile nobackup

set expandtab softtabstop=2 tabstop=2
set shiftwidth=2 shiftround 
set autoindent autowrite

set mouse=a

set incsearch hlsearch 

set foldenable
set foldmethod=manual
set foldlevelstart=0

syntax on
set number

if has("gui_gtk3")
  set guifont=Inconsolata\ 14
elseif has("gui_macvim")
  set guifont=Menlo\ Regular:h14
else
  set guifont=Consolas:h11:cANSI
endif

set showmatch
set backspace=indent,eol,start
set laststatus=2
set showmode
set scrolloff=5
set nowrap

" NOTE(Ryan): Although actually clang, has same errorformat as gcc
compiler gcc

if !exists('g:os')
  if has('win32') || has('win64')
    let g:os = 'Windows'
    autocmd GUIEnter * simalt ~x
  else
    let g:os = substitute(system('uname'), '\n', '', '')
  endif
endif


if g:os ==# 'Windows'
  let g:build_file_abs_path = fnamemodify(findfile("windows-build.bat", ";."), ":p:h")
  let &makeprg="cd " . g:build_file_abs_path . " && windows-build.bat"
elseif g:os ==# 'Linux'
  let g:build_file_abs_path = fnamemodify(findfile("linux-build.bash", ";."), ":p:h")
  let &makeprg="cd " . g:build_file_abs_path . " && bash linux-build.bash"
else
  let g:build_file_abs_path = fnamemodify(findfile("macos-build.bash", ";."), ":p:h")
  let &makeprg="cd " . g:build_file_abs_path . " && bash macos-build.bash"
endif
nnoremap <silent> <C-B> :make! <bar> copen <bar> redraw<CR>
nnoremap <silent> <C-N> :cnext<CR>
nnoremap <silent> <C-P> :cprev<CR>
nnoremap <silent> <C-C> :cclose<CR>

function! TabSelectOrPopupOrIndent()
  if col('.') == 1 || getline('.')[col('.') - 2] =~? '[ ]'
    return "\<Tab>"
  else
    return "\<C-X>\<C-N>"
  endif
endfunction
inoremap <expr> <Tab> TabSelectOrPopupOrIndent()
inoremap <expr> <CR> pumvisible() ? "\<C-Y>" : "\<CR>"
inoremap <expr> <Esc> pumvisible() ? "\<C-E>" : "\<Esc>"
inoremap <expr> n pumvisible() ? "\<C-N>" : 'n'
inoremap <expr> <S-N> pumvisible() ? "\<C-P>" : "\<S-N>"

nnoremap <S-F> :vimgrep //gj **/*.c **/*.h <bar> copen<C-Left><C-Left><C-Left><C-Left><C-Left><Right>
