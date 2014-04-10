RPM_PKGS = range(1) # pkg_type

class Swdb:

    def __init__(self, actors, path=None, default_type=None):
        """
        returns a new database handler
        Parameters
        ----------
        actors : list of HistoryPackage objects
            package managers that will participate during transactions
        path : str
            database path, should be used default
        default_type : int
            package type that will be used as default, don't have to be
            set field of HistoryPackage constructors
        """
        self.package = PackageHistoryQuery()
        self.transaction = TransactionHistoryQuery()
        self.package_transformation = PackageHistoryTransformation()


class HistoryQuery:
    def new(self, **kwargs):
        """
        returns a new object handler, object is saved after calling save method
        Parameters
        ----------
        kwargs : dict
            key is name of column and value is the value,
            that will be set for given column. Value can be str or int
        """

    def first(self):
        return self[0]

    def last(self):
        return self[-1]

    def all(self):
        return self.filter()

    def order_by(self, *columns):
        """
        order the result list by specific columns
        Parameters
        ----------
        columns : list of str
            they could start with '-' prefix to make descending order
        """

    def filter(self, **kwargs):
        """
        keeps in the resulting list only for columns (keys) that match given value
        """

    def exclude(self, **kwargs):
        """
        filter out in the resulting list records which columns (keys) match given value
        """

    def __list__(self):
        self.run()

    def __len__(self):
        len(self.run())

    def __getitem__(self, index):
        """
        slicing is used instead of limitby"""
        if isinstance(index, int):
            pass
        elif isinstance(index, slice):
            pass

    def run(self):
        """
        runs the query and returns list of items that fits filtered criteria
        """


class PackageHistoryQuery(HistoryQuery):

    def new(self, **kwargs):
        """
        constructor, should be used instead of initializing directly HistoryPackage object
        Parameters
        ----------
        pkg_type : int
        name : str
        version : str
        release : str
        arch : str
        **kwargs : dict
            any non-reserved column (key) and it's value str/int
        """


class TransactionHistoryQuery(HistoryQuery):

    def new(self, rpmdb_version=None, cmdline, pkgs=[]):
        """
        constructor, should be used instead of initializing directly HistoryTransaction object
        Parameters
        ----------
        cmdline : str
        rpmdb_version : str
        pkgs : list of HistoryPackageTransformation
        """


class PackageHistoryTransformation(HistoryQuery):

    def new(self, first_pkg, second_pkg, state, reason, group=None, done=False):
        """
        constructor, should be used instead of initializing directly PackageHistoryTransformation object
        the package pair is set by these rules:
            (`erased', erased\_pkg, `user', null)
            (`obsoleted', removed\_pkg, `dep', which\_obsoleted)
            (`installed', installed\_pkg, `user', null)
            (`installed', installed\_pkg, `dep', which\_requires)
            (`downgraded', downgraded\_pkg, \_, from\_pkg)
            (`upgraded', upgraded\_pkg, \_, from\_pkg)
            (`reinstalled', reinstalled\_pkg, \_, null)
        Parameters
        ----------
        first_pkg : HistoryPackage
        second_pkg : HistoryPackage
        state : str in ('erased', 'obsoleted', 'installed', 'downgraded', 'upgraded', 'reinstalled')
        reason : str in ('user', 'dep')
        group : int (groupid) / str (group name)
        done : True / False
        """


class Record:
    """
    Base class for HistoryTransaction and HistoryPackage.

    """

    @property
    def in_db(self):
        """
        returns True if object is already in database, returns False otherwise
        """

    @property
    def changed(self):
        """
        returns False whether object is stored in database with the same values
        as it holds now, otherwise returns True
        """

    def set(self, **kwargs):
        """
        sets the columns (keys) with given values
        ----------
        kwargs : dict
            key is name of column and value is the value,
            that will be set for given column. Value can be str or int
        """
        return self

    def get(self, key, default=None):
        """
        returns value of given attribute (key) or default value
        """

    def __getitem__(self, key):
        """
        returns value of given attribute (key) or throws KeyError
        """

    def __setitem__(self, key, value):
        """
        sets column (key) with given value
        """

    def create_index(self, field):
        pass


class PackageTransformation(Record):
    """
    Package handler for formerly known as trans_data_pkg from yum history database
    """


class HistoryTransaction(Record):
    """
    Logged transaction of package set, that will be upgraded, downgraded, installed,
    removed, reinstalled together
    """

    def set_all_pkgs(self, **kwargs):
        """
        sets columns (keys) with given values for all packages inserted in transaction
        """

    def save(self, transaction=True, pkgs=True):
        """
        saves the transaction and all package objects it holds to db
        """

    def add(self, pkgs):
        pass

    @staticmethod
    def reserved_fields(self):
        """
        returns list of tuples (column_name, type, which application uses it)
        """

    def merge(print_warnings=False, merging_rules=None):
        """
        reduce number of transactions that were executed many times
        E.g. installed, removed and reinstalled transaction of same package
        will result only to installed
        """

    def end(self, return_code=None):
        """
        runs save(), adds end_timestamp and mark all packages in transaction
        as done

        Parameters
        ----------
        return_code : int
            whats the result code of transaction as a whole (rpm, dnf results)
        """

class HistoryPackage(Record):
    """
    Package handler for formerly known yumdb
    """

    def save(self):
        """
        Writes all package attributes of object to database
        """

    def clear(self, fields=None):
        """
        Clears all non-package-identification data (only yumdb records)
        """

    @staticmethod
    def reserved_fields(self):
        """
        returns list of tuples (column_name, type, which application uses it)
        """
