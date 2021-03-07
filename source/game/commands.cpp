#include "commands.h"
#include "game.h"

void Chess::ICommand::Serialize(obytestream& stream, ICommand* command)
{
    stream << command->GetType();
    command->Write(stream);
}

Chess::ICommand* Chess::ICommand::Deserialize(ibytestream& stream)
{
    Type type;
    if (!(stream >> type))
    {
        return nullptr;
    }

    ICommand* cmd = nullptr;
    if (type == Type::Move)
        cmd = new MoveCommand;
    //else if (type == Type::Promote)
    //cmd = new ...

    bool read_res = false;
    if (cmd)
        read_res = cmd->Read(stream);

    assert(read_res);

    return cmd;
}

Chess::MoveCommand::MoveCommand()
{
}

Chess::MoveCommand::MoveCommand(Piece& piece, const Pos& pos)
{
    posFrom_ = piece.GetPos();
    posTo_ = pos;
}

Chess::MoveCommand::MoveCommand(const Pos& pos_from, const Pos& pos_to)
{
    posFrom_ = pos_from;
    posTo_ = pos_to;
}

bool Chess::MoveCommand::Validate(const Game& game) const
{
    auto piece = game.FindPieceAt(posFrom_);
    assert(piece);
    if (!piece)
        return false;

    if ((game.GetPlayerTurn() != piece->GetColor()))
        return false;

    auto available_movement = piece->GetMovement().GetAvailableMovement();
    if (std::find(available_movement.begin(), available_movement.end(), posTo_) == available_movement.end())
        return false;

    if (posFrom_ == posTo_)
        return false;

    auto piece_enemy = game.FindPieceAt(posTo_);
    if (piece_enemy && piece_enemy->GetColor() == piece->GetColor())
        return false;

    return true;
}

void Chess::MoveCommand::Do(Game& game)
{
    auto piece = game.FindPieceAt(posFrom_);
    assert(piece);
    if (!piece)
        return;

    if (auto enemy = game.FindPieceAt(posTo_))
    {
        enemyPiece_ = enemy->Serialize();
        game.RemovePiece(*enemy);
    }
    pieceType_= piece->GetType();
    pieceColor_=piece->GetColor();
    piece->SetPos(posTo_);
    pieceHasMovedBefore_ = piece->HasMoved();
    piece->SetHasMoved(true);
}

void Chess::MoveCommand::Undo(Game& game)
{
    auto piece = game.FindPieceAt(posTo_);
    assert(piece);
    if (!piece)
        return;

    piece->SetPos(posFrom_);
    piece->SetHasMoved(pieceHasMovedBefore_);

    if (enemyPiece_)
    {
        auto enemy = new Piece(*enemyPiece_);
        game.AddPiece(*enemy);
        enemy->SetPos(posTo_);
    }
}

Chess::ICommand::Type Chess::MoveCommand::GetType() const
{
    return Type::Move;
}

std::string Chess::MoveCommand::ToString() const
{
    std::string text;

    if (pieceType_ == PieceType::Knight)
        text += 'N';
    else if (pieceType_ ==PieceType::Bishop)
        text += 'B';
    else if (pieceType_ ==PieceType::Rook)
        text += 'R';
    else if (pieceType_ ==PieceType::Queen)
        text += 'Q';
    else if (pieceType_ ==PieceType::King)
        text += 'K';

    if (enemyPiece_)
    {
        if (pieceType_ == PieceType::Pawn)
            text += posFrom_.XToChar();
        text += 'x';
    }

    text += posTo_.ToString();

    return text;
}

bool Chess::MoveCommand::IsFromReplication() const
{
    return fromReplication_;
}

void Chess::MoveCommand::MarkFromReplication()
{
    fromReplication_ = true;
}

void Chess::MoveCommand::Write(obytestream& stream) const
{
    stream << posFrom_;
    stream << posTo_;
    stream << pieceType_;
    stream << pieceColor_;
    stream << pieceHasMovedBefore_;
    stream << enemyPiece_;
}

bool Chess::MoveCommand::Read(ibytestream& stream)
{
    if (!(stream >> posFrom_))
        return false;
    if (!(stream >> posTo_))
        return false;
    if (!(stream >> pieceType_))
        return false;
    if (!(stream >> pieceColor_))
        return false;
    if (!(stream >> pieceHasMovedBefore_))
        return false;
    if (!(stream >> enemyPiece_))
        return false;

    return true;
}

//////////////////////ReplacePieceCommand//////////////////////////

Chess::ReplacePieceCommand::ReplacePieceCommand(Pos pos, PieceType replace_type)
    : pos_(pos)
    , replaceType_(replace_type)
{
}

bool Chess::ReplacePieceCommand::Validate(const Game& game) const
{
    return true;
}

void Chess::ReplacePieceCommand::Do(Game& game)
{
    auto piece = game.FindPieceAt(pos_);
    if (!piece)
        return;

    auto color = piece->GetColor();
    replacedType_ = piece->GetType();
    game.RemovePiece(*piece);
    auto new_piece = new Piece(replaceType_, color);
    game.AddPiece(*new_piece);
    new_piece->SetPos(pos_);
}

void Chess::ReplacePieceCommand::Undo(Game& game)
{
    auto piece = game.FindPieceAt(pos_);
    if (!piece)
        return;

    auto color = piece->GetColor();
    game.RemovePiece(*piece);
    auto new_piece = new Piece(replacedType_, color);
    game.AddPiece(*new_piece);
    new_piece->SetPos(pos_);
}

Chess::ICommand::Type Chess::ReplacePieceCommand::GetType() const
{
    return ICommand::Type::Replace;
}

std::string Chess::ReplacePieceCommand::ToString() const
{
    return "";
}

bool Chess::ReplacePieceCommand::IsFromReplication() const
{
    return isFromReplication_;
}

void Chess::ReplacePieceCommand::MarkFromReplication()
{
    isFromReplication_ = true;
}

void Chess::ReplacePieceCommand::Write(obytestream& stream) const
{
    stream << pos_;
    stream << replaceType_;
    stream << replacedType_;
}

bool Chess::ReplacePieceCommand::Read(ibytestream& stream)
{
    if (!(stream >> pos_))
        return false;
    if (!(stream >> replaceType_))
        return false;
    if (!(stream >> replacedType_))
        return false;

    return true;
}

//////////////////////CompoundCommand/////////////////////////////

Chess::CompoundCommand::CompoundCommand(std::vector<ICommandUnPtr>&& commands)
{
    commands_ = std::move(commands);
}

bool Chess::CompoundCommand::Validate(const Game& game) const
{
    for (auto& command : commands_)
        if (!command->Validate(game))
            return false;

    return true;
}

void Chess::CompoundCommand::Do(Game& game)
{
    for (auto& command : commands_)
        command->Do(game);
}

void Chess::CompoundCommand::Undo(Game& game)
{
    for (auto it = commands_.rbegin(); it != commands_.rend(); ++it)
        (*it)->Undo(game);
}

Chess::ICommand::Type Chess::CompoundCommand::GetType() const
{
    return Type::Compound;
}

std::string Chess::CompoundCommand::ToString() const
{
    return "";
}

bool Chess::CompoundCommand::IsFromReplication() const
{
    return fromReplication_;
}

void Chess::CompoundCommand::MarkFromReplication()
{
    fromReplication_ = true;
}

void Chess::CompoundCommand::Write(obytestream& stream) const
{
    stream << (int)commands_.size();
    for (auto& command : commands_)
        ICommand::Serialize(stream, command.get());
}

bool Chess::CompoundCommand::Read(ibytestream& stream)
{
    int num_commands = 0;
    if (!(stream >> num_commands))
        return false;

    for (int i = 0; i < num_commands; i++)
    {
        auto command = ICommand::Deserialize(stream);
        if (!command)
            return false;

        commands_.push_back(std::unique_ptr<ICommand>(command));
    }

    return true;
}

