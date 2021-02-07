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
    if (pieceType_ == PieceType::Pawn)
        return posFrom_.ToString() + "->" + posTo_.ToString();
    else if (pieceType_ ==PieceType::Knight)
        return "N" + posFrom_.ToString() + "->" + posTo_.ToString();
    else if (pieceType_ ==PieceType::Bishop)
        return "B" +posFrom_.ToString() + "->" + posTo_.ToString();
    else if (pieceType_ ==PieceType::Rook)
        return "R" +  posFrom_.ToString() + "->" +posTo_.ToString();
    else if (pieceType_ ==PieceType::Queen)
        return "Q" + posFrom_.ToString() + "->" +posTo_.ToString();
    else if (pieceType_ ==PieceType::King)
        return "K" + posFrom_.ToString() + "->" +posTo_.ToString();
    return "";
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
    stream << kingUnderAtak_;
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
    if (!(stream >> kingUnderAtak_))
        return false;
    if (!(stream >> enemyPiece_))
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

