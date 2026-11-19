const CORREDORES = 3;
const VAGAS_POR_CORREDOR = 5;

// mesma estrutura do programa em C: matriz de corredores x vagas
let estacionamento = [];

function letraCorredor(c) {
    return String.fromCharCode(65 + c);
}

function inicializarVagas() {
    for (let c = 0; c < CORREDORES; c++) {
        estacionamento[c] = [];
        for (let v = 0; v < VAGAS_POR_CORREDOR; v++) {
            estacionamento[c][v] = { numero: v + 1, ocupada: false, veiculo: null };
        }
    }
}

function contarLivresCorredor(c) {
    let livres = 0;
    for (let v = 0; v < VAGAS_POR_CORREDOR; v++) {
        if (!estacionamento[c][v].ocupada) livres++;
    }
    return livres;
}

// escolhe o corredor com mais vagas livres e a primeira vaga livre dele
function buscarVagaLivre() {
    let melhor = -1;
    let maiorLivres = 0;

    for (let c = 0; c < CORREDORES; c++) {
        let livres = contarLivresCorredor(c);
        if (livres > maiorLivres) {
            maiorLivres = livres;
            melhor = c;
        }
    }

    if (melhor == -1) return null;

    for (let v = 0; v < VAGAS_POR_CORREDOR; v++) {
        if (!estacionamento[melhor][v].ocupada) {
            return { c: melhor, v: v };
        }
    }
    return null;
}

function buscarPlaca(placa) {
    for (let c = 0; c < CORREDORES; c++) {
        for (let v = 0; v < VAGAS_POR_CORREDOR; v++) {
            let vaga = estacionamento[c][v];
            if (vaga.ocupada && vaga.veiculo.placa == placa) {
                return { c: c, v: v };
            }
        }
    }
    return null;
}

function horaFormatada(data) {
    return data.toLocaleTimeString('pt-BR', { hour: '2-digit', minute: '2-digit' });
}

// tipo: 'sucesso' ou 'erro' (muda a cor da caixa)
function mensagem(texto, tipo) {
    let caixa = document.getElementById('mensagem');
    caixa.textContent = texto;
    caixa.className = tipo;
    caixa.hidden = false;
}

const TEXTO_DETALHE = 'Clique em uma vaga ocupada para ver o veículo.';
let vagaSelecionada = null;
let vagaNova = null;

function atualizarIndicadores() {
    let total = CORREDORES * VAGAS_POR_CORREDOR;
    let livres = 0;
    for (let c = 0; c < CORREDORES; c++) {
        livres += contarLivresCorredor(c);
    }
    let ocupacao = (total - livres) * 100 / total;

    document.getElementById('total').textContent = total;
    document.getElementById('livres').textContent = livres;
    document.getElementById('ocupadas').textContent = total - livres;
    document.getElementById('ocupacao').textContent = ocupacao.toFixed(1) + '%';

    // barra fica verde, amarela ou vermelha conforme lota
    let barra = document.getElementById('barraOcupacao');
    barra.style.width = ocupacao + '%';
    if (ocupacao >= 80) barra.style.background = '#d64545';
    else if (ocupacao >= 50) barra.style.background = '#e0a800';
    else barra.style.background = '#2e9e5b';
}

function mostrarDetalhe(c, v) {
    let vaga = estacionamento[c][v];
    let carro = vaga.veiculo;
    let minutos = Math.round((new Date() - carro.entrada) / 60000);

    let detalhe = document.getElementById('detalhe');
    detalhe.innerHTML = '';

    let titulo = document.createElement('strong');
    titulo.textContent = 'Vaga ' + letraCorredor(c) + vaga.numero + ' — ' + carro.placa;
    detalhe.appendChild(titulo);
    detalhe.appendChild(document.createElement('br'));
    detalhe.appendChild(document.createTextNode(
        carro.modelo + ' ' + carro.cor + ' · entrada às ' + horaFormatada(carro.entrada) +
        ' (' + minutos + ' min)'));

    // já deixa a placa pronta no formulário de saída
    document.getElementById('placaSaida').value = carro.placa;
}

function mostrarMapa() {
    let mapa = document.getElementById('mapa');
    mapa.innerHTML = '';

    for (let c = 0; c < CORREDORES; c++) {
        let linha = document.createElement('div');
        linha.className = 'corredor';

        let nome = document.createElement('div');
        nome.className = 'nome';
        nome.textContent = 'Corredor ' + letraCorredor(c);
        let livres = document.createElement('small');
        livres.textContent = contarLivresCorredor(c) + ' livre(s)';
        nome.appendChild(livres);
        linha.appendChild(nome);

        for (let v = 0; v < VAGAS_POR_CORREDOR; v++) {
            let vaga = estacionamento[c][v];
            let codigo = letraCorredor(c) + vaga.numero;

            let div = document.createElement('div');
            div.className = 'vaga ' + (vaga.ocupada ? 'ocupada' : 'livre');
            div.textContent = codigo;

            let info = document.createElement('small');
            info.textContent = vaga.ocupada ? vaga.veiculo.placa : 'livre';
            div.appendChild(info);

            if (vaga.ocupada) {
                div.title = vaga.veiculo.modelo + ' ' + vaga.veiculo.cor;
                if (vagaSelecionada == codigo) div.classList.add('selecionada');
                if (vagaNova == codigo) div.classList.add('nova');

                div.onclick = function () {
                    vagaSelecionada = codigo;
                    vagaNova = null;
                    mostrarDetalhe(c, v);
                    mostrarMapa();
                };
            }
            linha.appendChild(div);
        }
        mapa.appendChild(linha);
    }
}

function atualizarTela() {
    mostrarMapa();
    atualizarIndicadores();
}

document.getElementById('formEntrada').onsubmit = function (e) {
    e.preventDefault();

    let carro = {
        placa: document.getElementById('placa').value.trim().toUpperCase(),
        modelo: document.getElementById('modelo').value.trim(),
        cor: document.getElementById('cor').value.trim(),
        entrada: new Date()
    };

    let existe = buscarPlaca(carro.placa);
    if (existe) {
        mensagem('⚠️ O veículo ' + carro.placa + ' já está na vaga ' + letraCorredor(existe.c) + (existe.v + 1) + '.', 'erro');
        return;
    }

    let pos = buscarVagaLivre();
    if (!pos) {
        mensagem('🚫 Estacionamento lotado! Não há vagas livres no momento.', 'erro');
        return;
    }

    estacionamento[pos.c][pos.v].ocupada = true;
    estacionamento[pos.c][pos.v].veiculo = carro;
    vagaNova = letraCorredor(pos.c) + (pos.v + 1);

    mensagem('✅ ' + carro.placa + ' estacionado na vaga ' + vagaNova +
             ' às ' + horaFormatada(carro.entrada) + '.', 'sucesso');
    this.reset();
    document.getElementById('placa').focus();
    atualizarTela();
};

document.getElementById('formSaida').onsubmit = function (e) {
    e.preventDefault();

    let placa = document.getElementById('placaSaida').value.trim().toUpperCase();
    let pos = buscarPlaca(placa);
    if (!pos) {
        mensagem('⚠️ Nenhum veículo com a placa ' + placa + ' está no estacionamento.', 'erro');
        return;
    }

    let vaga = estacionamento[pos.c][pos.v];
    let minutos = Math.round((new Date() - vaga.veiculo.entrada) / 60000);

    vaga.ocupada = false;
    vaga.veiculo = null;
    vagaSelecionada = null;
    vagaNova = null;

    mensagem('👋 ' + placa + ' saiu da vaga ' + letraCorredor(pos.c) + vaga.numero +
             '. Tempo de permanência: ' + minutos + ' minuto(s).', 'sucesso');
    document.getElementById('detalhe').textContent = TEXTO_DETALHE;
    this.reset();
    atualizarTela();
};

inicializarVagas();
atualizarTela();
